package org.cismypa.wealthplanner;

import android.app.Activity;
import android.content.Context;
import android.util.Log;

import com.android.billingclient.api.AcknowledgePurchaseParams;
import com.android.billingclient.api.BillingClient;
import com.android.billingclient.api.BillingClientStateListener;
import com.android.billingclient.api.BillingFlowParams;
import com.android.billingclient.api.BillingResult;
import com.android.billingclient.api.ProductDetails;
import com.android.billingclient.api.Purchase;
import com.android.billingclient.api.PurchasesUpdatedListener;
import com.android.billingclient.api.QueryProductDetailsParams;
import com.android.billingclient.api.QueryPurchasesParams;

import java.util.Collections;
import java.util.List;

public class BillingHelper implements PurchasesUpdatedListener {
    private static final String TAG = "BillingHelper";
    private static final String PRODUCT_ID = "remove_ads";

    private static BillingHelper instance;
    private BillingClient billingClient;
    private Activity activity;
    private ProductDetails removeAdsProduct;

    public static void initialize(Context context) {
        if (!(context instanceof Activity)) {
            Log.e(TAG, "Context is not an Activity");
            return;
        }
        instance = new BillingHelper();
        instance.activity = (Activity) context;
        instance.connect();
    }

    private void connect() {
        billingClient = BillingClient.newBuilder(activity)
                .setListener(this)
                .enablePendingPurchases()
                .build();

        billingClient.startConnection(new BillingClientStateListener() {
            @Override
            public void onBillingSetupFinished(BillingResult result) {
                if (result.getResponseCode() == BillingClient.BillingResponseCode.OK) {
                    Log.d(TAG, "Billing client connected");
                    queryExistingPurchases();
                } else {
                    Log.w(TAG, "Billing setup failed: " + result.getDebugMessage());
                }
            }

            @Override
            public void onBillingServiceDisconnected() {
                Log.w(TAG, "Billing service disconnected");
            }
        });
    }

    private void queryExistingPurchases() {
        QueryPurchasesParams params = QueryPurchasesParams.newBuilder()
                .setProductType(BillingClient.ProductType.INAPP)
                .build();

        billingClient.queryPurchasesAsync(params, (result, purchases) -> {
            if (result.getResponseCode() == BillingClient.BillingResponseCode.OK) {
                for (Purchase purchase : purchases) {
                    if (purchase.getProducts().contains(PRODUCT_ID)
                            && purchase.getPurchaseState() == Purchase.PurchaseState.PURCHASED) {
                        Log.d(TAG, "Existing remove_ads purchase found");
                        acknowledgePurchase(purchase);
                        nativeOnPurchaseResult(true);
                        return;
                    }
                }
            }
        });
    }

    public static void launchPurchaseFlow(Context context) {
        if (instance == null || instance.billingClient == null) {
            Log.e(TAG, "BillingHelper not initialized");
            nativeOnPurchaseResult(false);
            return;
        }
        if (!(context instanceof Activity)) {
            nativeOnPurchaseResult(false);
            return;
        }

        instance.activity = (Activity) context;

        if (!instance.billingClient.isReady()) {
            Log.w(TAG, "Billing client not ready, reconnecting");
            instance.connect();
            nativeOnPurchaseResult(false);
            return;
        }

        QueryProductDetailsParams queryParams = QueryProductDetailsParams.newBuilder()
                .setProductList(Collections.singletonList(
                        QueryProductDetailsParams.Product.newBuilder()
                                .setProductId(PRODUCT_ID)
                                .setProductType(BillingClient.ProductType.INAPP)
                                .build()))
                .build();

        instance.billingClient.queryProductDetailsAsync(queryParams, (result, productDetailsList) -> {
            if (result.getResponseCode() != BillingClient.BillingResponseCode.OK
                    || productDetailsList.isEmpty()) {
                Log.e(TAG, "Failed to query product details: " + result.getDebugMessage());
                nativeOnPurchaseResult(false);
                return;
            }

            instance.removeAdsProduct = productDetailsList.get(0);

            BillingFlowParams flowParams = BillingFlowParams.newBuilder()
                    .setProductDetailsParamsList(Collections.singletonList(
                            BillingFlowParams.ProductDetailsParams.newBuilder()
                                    .setProductDetails(instance.removeAdsProduct)
                                    .build()))
                    .build();

            instance.activity.runOnUiThread(() -> {
                BillingResult flowResult = instance.billingClient.launchBillingFlow(
                        instance.activity, flowParams);
                if (flowResult.getResponseCode() != BillingClient.BillingResponseCode.OK) {
                    Log.e(TAG, "Launch billing flow failed: " + flowResult.getDebugMessage());
                    nativeOnPurchaseResult(false);
                }
            });
        });
    }

    @Override
    public void onPurchasesUpdated(BillingResult result, List<Purchase> purchases) {
        if (result.getResponseCode() == BillingClient.BillingResponseCode.OK && purchases != null) {
            for (Purchase purchase : purchases) {
                if (purchase.getPurchaseState() == Purchase.PurchaseState.PURCHASED) {
                    acknowledgePurchase(purchase);
                    nativeOnPurchaseResult(true);
                    return;
                }
            }
        } else if (result.getResponseCode() == BillingClient.BillingResponseCode.USER_CANCELED) {
            Log.d(TAG, "User cancelled purchase");
        } else {
            Log.w(TAG, "Purchase failed: " + result.getDebugMessage());
        }
        nativeOnPurchaseResult(false);
    }

    private void acknowledgePurchase(Purchase purchase) {
        if (purchase.isAcknowledged()) return;

        AcknowledgePurchaseParams params = AcknowledgePurchaseParams.newBuilder()
                .setPurchaseToken(purchase.getPurchaseToken())
                .build();

        billingClient.acknowledgePurchase(params, result -> {
            if (result.getResponseCode() == BillingClient.BillingResponseCode.OK) {
                Log.d(TAG, "Purchase acknowledged");
            } else {
                Log.w(TAG, "Acknowledge failed: " + result.getDebugMessage());
            }
        });
    }

    private static native void nativeOnPurchaseResult(boolean success);
}
