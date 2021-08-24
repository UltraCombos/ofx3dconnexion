#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofx3dcon.setup();
	auto name = ofx3dcon.get_device_name();
	printf("device_name = %s\n", name.c_str());
	ofAddListener(ofx3dcon.get_event_motion(), this, &ofApp::on_motion_event);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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

void ofApp :: exit()
{
	ofx3dcon.exit();

}
void ofApp::on_motion_event(ofx3Dconnexion::MotionEventArgs& args)
{
	printf("args.rotation = %f, %f, %f, translation = %f, %f, %f\n", args.rotation.x, args.rotation.y, args.rotation.z, args.translate.x, args.translate.y, args.translate.z);
	
}