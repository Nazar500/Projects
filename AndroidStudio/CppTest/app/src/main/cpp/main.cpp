#include <jni.h>
#include <android/log.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>

// EGL variables
EGLDisplay eglDisplay;
EGLSurface eglSurface;
EGLContext eglContext;
EGLConfig eglConfig;

// Initialize EGL
void initEGL(ANativeWindow* window) {
    eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(eglDisplay, NULL, NULL);

    // Choose the EGL configuration
    EGLint configAttribs[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_CONFORMANT, EGL_OPENGL_ES2_BIT,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_DEPTH_SIZE, 16,
            EGL_STENCIL_SIZE, 8,
            EGL_NONE
    };

    EGLint numConfigs;
    eglChooseConfig(eglDisplay, configAttribs, &eglConfig, 1, &numConfigs);

    // Create an EGL window surface
    eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, window, NULL);

    // Create an EGL rendering context
    EGLint contextAttribs[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_NONE
    };

    eglContext = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, contextAttribs);

    // Make the context current
    eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);

    // Set the clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

// Swap buffers
void swapBuffers() {
    eglSwapBuffers(eglDisplay, eglSurface);
}

// Cleanup EGL resources
void cleanupEGL() {
    eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroyContext(eglDisplay, eglContext);
    eglDestroySurface(eglDisplay, eglSurface);
    eglTerminate(eglDisplay);
}

void android_main(struct android_app* app) {
    app_dummy();

    ANativeWindow* window = app->window;
    if (window == nullptr) {
        LOGE("Native window is null.");
        return;
    }

    // Initialize EGL
    initEGL(window);

    // Set up shaders and other resources
    setupShaders();

    // Main loop
    while (1) {
        int events;
        android_poll_source* source;
        while (ALooper_pollAll(0, NULL, &events, (void**)&source) >= 0) {
            if (source != NULL) {
                source->process(app, source);
            }

            // Render the calculator UI
            renderCalculator();

            // Swap buffers
            swapBuffers();
        }
    }

    // Cleanup
    cleanupEGL();
}
