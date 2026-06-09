package org.stellarlib.app;

import android.content.Context;
import org.libsdl.app.SDLActivity;
import org.libsdl.app.SDLSurface;

public class SLActivity extends SDLActivity {

    @Override
    protected SDLSurface createSDLSurface(Context context) {
        return new SLSurface(context);
    }
}
