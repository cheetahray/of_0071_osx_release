#include "ofAppGlutWindow.h"
#include "testApp.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, rayx, rayy+10, OF_FULLSCREEN);
	ofRunApp(new testApp());
}
