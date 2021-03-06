#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofBackground(50);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofEnableSmoothing();

    // Set up camera
    vidGrabber.setVerbose(true);
    vector<ofVideoDevice> devices = vidGrabber.listDevices();
    vidGrabber.setDeviceID(devices[devices.size()-1].id);
    vidGrabber.initGrabber(320, 240);

    grayImage.allocate(320, 240);
    imageDiff.allocate(320, 240);
    smallImageDiff.allocate(60, 40);
    vf.setupField(60, 40, 360, 240);
    
    showCamera = false;

    int x = (ofGetWidth() - table.TABLE_WIDTH) * 0.5;       // center on screen.
    int y = (ofGetHeight() - table.TABLE_HEIGHT) * 0.5;     // center on screen.
    int w = table.TABLE_WIDTH;
    int h = table.TABLE_HEIGHT;
    
    warper.setSourceRect(ofRectangle(0, 0, w, h));              // this is the source rectangle which is the size of the image and located at ( 0, 0 )
    warper.setTopLeftCornerPosition(ofPoint(x, y));             // this is position of the quad warp corners, centering the image on the screen.
    warper.setTopRightCornerPosition(ofPoint(x + w, y));        // this is position of the quad warp corners, centering the image on the screen.
    warper.setBottomLeftCornerPosition(ofPoint(x, y + h));      // this is position of the quad warp corners, centering the image on the screen.
    warper.setBottomRightCornerPosition(ofPoint(x + w, y + h)); // this is position of the quad warp corners, centering the image on the screen.
    warper.setup();
    warper.load(); // reload last saved changes.
    
    isToGetBaseImage = true;
    threshold = 200;
}

//--------------------------------------------------------------
void ofApp::update()
{
    if(ofGetFrameNum() % 5 != 0) {
        // only update every 5 frames.
        return;
    }

    table.update();
    
    vidGrabber.update();
    if (vidGrabber.isFrameNew()) {  // if camera image is updated
        colorImage.setFromPixels(vidGrabber.getPixels(), 320, 240);
        grayImage = colorImage;  // convert to gray scale
    }
    
    if (isToGetBaseImage) {
        baseImage = grayImage;
        isToGetBaseImage = false;
    } else {
        imageDiff.absDiff(baseImage, grayImage);
        imageDiff.threshold(threshold);
        smallImageDiff.scaleIntoMe(imageDiff);
        smallImageDiff.blur(5);
    }

    vf.setFromPixels(smallImageDiff.getPixels(), false, 0.05f);
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetColor(255);
    
    
    //======================== get our quad warp matrix.
    
    ofMatrix4x4 mat = warper.getMatrix();

    glPushMatrix();
    glMultMatrixf(mat.getPtr());

    table.draw();

    glPopMatrix();

    //========================
    if (showCamera) {
        cameraFbo.begin();
        colorImage.draw(0, 0);
        ofSetColor(255, 255, 255, 127);
        vf.draw();
        cameraFbo.end();
        cameraFbo.draw(0, 0);
    }
    
    //======================== draw quad warp ui.
    
    ofSetColor(ofColor::magenta);
    warper.drawQuadOutline();
    
    ofSetColor(ofColor::yellow);
    warper.drawCorners();
    
    ofSetColor(ofColor::magenta);
    warper.drawHighlightedCorner();
    
    ofSetColor(ofColor::red);
    warper.drawSelectedCorner();
    
    //======================== info.

    if (showHelp) {
        ofSetColor(ofColor::white);
        ofDrawBitmapString("to warp the image, drag the corners of the image.", 20, 30);
        ofDrawBitmapString("press 's' to toggle quad warp UI. this will also disable quad warp interaction.", 20, 50);
        ofDrawBitmapString("press & hold 1, 2, 3, 4 to snap that point to the mouse", 20, 70);
        ofDrawBitmapString("when a corner is selected (red), use keyboard arrow keys to nudge the corner position.", 20, 90);
    }
}

void ofApp::exit() {
    warper.save();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

    if (key == 's' || key == 'S') {
        warper.toggleShow();
    }

    if (key == 'c' || key == 'C') {
        showCamera = !showCamera;
    }

    
    if (key == 'l' || key == 'L') {
        warper.load();
    }
    
    if (key == 'h' || key == 'H') {
        warper.save();
    }

    if (key == 'f' || key == 'F') {
        table.swingRacket(+1);
    }

    if (key == 'j' || key == 'J') {
        table.swingRacket(-1);
    }

    if (key == '/' || key == '?') {
        showHelp = !showHelp;
    }

    if (key == ' ') {
        table.init();
    }
    
//    if (key == OF_KEY_LEFT) {
//        ball.velocity.x += -10;
//    } else if (key == OF_KEY_RIGHT) {
//        ball.velocity.x += 10;
//    }
//    
//    if (key == OF_KEY_UP) {
//        ball.velocity.y += -10;
//    } else if (key == OF_KEY_DOWN) {
//        ball.velocity.y += 10;
//    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
