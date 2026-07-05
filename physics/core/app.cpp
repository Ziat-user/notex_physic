// app.cpp
#include "app.h"
#include "Application.h"

int RunApplication(HINSTANCE hInstance, int nCmdShow)
{
    Application app;
    return app.Run(hInstance, nCmdShow);
}

// app.cpp