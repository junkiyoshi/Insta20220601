#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofEnableDepthTest();

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {

	this->face.clear();
	this->frame.clear();

	this->cam.begin();

	auto radius = 200;

	ofSetColor(0);
	ofDrawSphere(radius);

	for (radius = radius + 50; radius < 300; radius += 5) {

		this->setRingToMesh(this->face, this->frame, glm::vec3(), radius, radius * 0.1, ofColor(0), ofColor(255));
	}

	this->face.draw();
	this->frame.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
void ofApp::setRingToMesh(ofMesh& face_target, ofMesh& frame_target, glm::vec3 location, float radius, float height, ofColor face_color, ofColor frame_color) {

	int index = face_target.getNumVertices();

	for (int deg = 0; deg < 360; deg += 5) {

		vector<glm::vec3> vertices;
		vertices.push_back(glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3(radius * cos((deg + 10) * DEG_TO_RAD), radius * sin((deg + 10) * DEG_TO_RAD), height * -0.5));
		vertices.push_back(glm::vec3(radius * cos((deg + 10) * DEG_TO_RAD), radius * sin((deg + 10) * DEG_TO_RAD), height * 0.5));
		vertices.push_back(glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), height * 0.5));

		for (auto& vertex : vertices) {

			auto noise_value_x = ofNoise(
				location.x, radius * 0.001 + ofGetFrameNum() * 0.003);
			auto noise_value_y = ofNoise(
				location.y, radius * 0.001 + ofGetFrameNum() * 0.003);
			auto noise_value_z = ofNoise(
				location.z, radius * 0.001 + ofGetFrameNum() * 0.003);

			auto rotation_x = glm::rotate(glm::mat4(), ofMap(noise_value_x, 0, 1, -PI * 2.5, PI * 2.5), glm::vec3(1, 0, 0));
			auto rotation_y = glm::rotate(glm::mat4(), ofMap(noise_value_y, 0, 1, -PI * 2.5, PI * 2.5), glm::vec3(0, 1, 0));
			auto rotation_z = glm::rotate(glm::mat4(), ofMap(noise_value_z, 0, 1, -PI * 2.5, PI * 2.5), glm::vec3(0, 0, 1));

			vertex = glm::vec4(vertex, 0) * rotation_y * rotation_x + glm::vec4(location, 0);
		}

		auto face_index = face_target.getNumVertices();
		face_target.addVertices(vertices);

		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 1); face_target.addIndex(face_index + 2);
		face_target.addIndex(face_index + 0); face_target.addIndex(face_index + 2); face_target.addIndex(face_index + 3);

		auto frame_index = frame_target.getNumVertices();
		frame_target.addVertices(vertices);

		frame_target.addIndex(frame_index + 0); frame_target.addIndex(frame_index + 1);
		frame_target.addIndex(frame_index + 2); frame_target.addIndex(frame_index + 3);

		for (int i = 0; i < vertices.size(); i++) {

			face_target.addColor(face_color);
			frame_target.addColor(frame_color);
		}
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}