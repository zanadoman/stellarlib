package org.stellarlib.app;

import android.content.Context;
import android.graphics.Insets;
import android.os.Build;
import android.view.View;
import android.view.WindowInsets;
import org.libsdl.app.SDLActivity;
import org.libsdl.app.SDLSurface;

public class SLSurface extends SDLSurface {

    protected SLSurface(Context context) {
        super(context);
    }

    @Override
    public WindowInsets onApplyWindowInsets(View v, WindowInsets insets) {
        if (Build.VERSION.SDK_INT < 30) {
            return insets;
        }

        Insets combined = insets.getInsets(WindowInsets.Type.displayCutout() | WindowInsets.Type.systemBars());
        SDLActivity.onNativeInsetsChanged(combined.left, combined.right, combined.top, combined.bottom);

        if (insets.isVisible(WindowInsets.Type.ime())) {
            if (!mKeyboardVisible) {
                mKeyboardVisible = true;
                SDLActivity.onNativeScreenKeyboardShown();
            }
        } else if (mKeyboardVisible) {
            mKeyboardVisible = false;
            SDLActivity.onNativeScreenKeyboardHidden();
        }

        return insets;
    }
}
