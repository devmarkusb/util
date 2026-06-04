# Qt Android bindings and JNI entry points.
-keep class org.qtproject.qt.** { *; }
-keepclasseswithmembernames class * {
    native <methods>;
}

# App Java layer invoked from C++ via JNI.
-keep class org.cismypa.wealthplanner.** { *; }

# Keep manifest-declared components and metadata used via reflection.
-keepclassmembers class * {
    @android.webkit.JavascriptInterface <methods>;
}

-keepattributes *Annotation*,Signature,InnerClasses,EnclosingMethod

# Play Services / Billing ship consumer rules in their AARs; suppress benign warnings.
-dontwarn com.google.android.gms.**
-dontwarn com.android.billingclient.**
