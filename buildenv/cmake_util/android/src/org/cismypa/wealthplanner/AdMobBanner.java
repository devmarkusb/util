package org.cismypa.wealthplanner;

import android.app.Activity;
import android.content.Context;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Gravity;
import android.view.ViewGroup;
import android.widget.FrameLayout;

import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.LoadAdError;
import com.google.android.gms.ads.MobileAds;

public class AdMobBanner {
    private static final String TAG = "AdMobBanner";
    private static AdView adView;

    public static void initialize(Context context, String adUnitId) {
        if (!(context instanceof Activity)) {
            Log.e(TAG, "Context is not an Activity");
            return;
        }
        Activity activity = (Activity) context;

        MobileAds.initialize(activity, initializationStatus ->
                Log.d(TAG, "Mobile Ads SDK initialized"));

        activity.runOnUiThread(() -> createAndShowBanner(activity, adUnitId));
    }

    private static void createAndShowBanner(Activity activity, String adUnitId) {
        try {
            adView = new AdView(activity);
            adView.setAdUnitId(adUnitId);

            DisplayMetrics dm = activity.getResources().getDisplayMetrics();
            int adWidthDp = (int) (dm.widthPixels / dm.density);
            AdSize adSize = AdSize.getCurrentOrientationAnchoredAdaptiveBannerAdSize(
                    activity, adWidthDp);
            adView.setAdSize(adSize);

            adView.setAdListener(new AdListener() {
                @Override
                public void onAdLoaded() {
                    Log.d(TAG, "Banner ad loaded");
                }

                @Override
                public void onAdFailedToLoad(LoadAdError error) {
                    Log.w(TAG, "Banner ad failed to load: " + error.getMessage());
                }
            });

            FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(
                    FrameLayout.LayoutParams.MATCH_PARENT,
                    FrameLayout.LayoutParams.WRAP_CONTENT,
                    Gravity.BOTTOM | Gravity.CENTER_HORIZONTAL);

            ViewGroup contentView = activity.findViewById(android.R.id.content);
            if (contentView != null) {
                contentView.addView(adView, params);
            } else {
                Log.e(TAG, "Could not find content view");
                return;
            }

            AdRequest adRequest = new AdRequest.Builder().build();
            adView.loadAd(adRequest);
        } catch (Exception e) {
            Log.e(TAG, "Failed to create banner ad", e);
        }
    }
}
