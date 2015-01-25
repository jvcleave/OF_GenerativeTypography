#include "ofApp.h"

bool doUpdateDrawMode =false;
//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetBackgroundAuto(false);
    
    ofEnableAntiAliasing();
    ofSetFrameRate(60); // cap frameRate otherwise it goes too fast
    
	maxParticles = 10000; // the maximum number of active particles
	drawMode = 0; // move through the drawing modes by clicking the mouse

	bg_color = ofColor(255);
	fbo_color = ofColor(0);
    
    
    ofTrueTypeFont ttf;
    ttf.loadFont(OF_TTF_SANS, 350);
    string s = "TYPE";
    
	fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    pix.allocate(ofGetWidth(), ofGetHeight(), OF_PIXELS_RGBA);
    fbo.begin();
    ofClear(0, 0, 0, 0);
	
	// Center string code from:
	// https://github.com/armadillu/ofxCenteredTrueTypeFont/blob/master/src/ofxCenteredTrueTypeFont.h
	ofRectangle r = ttf.getStringBoundingBox(s, 0, 0);
	ofVec2f offset = ofVec2f(floor(-r.x - r.width * 0.5f), floor(-r.y - r.height * 0.5f));
	ofSetColor(fbo_color);
	ttf.drawString(s, fbo.getWidth() / 2 + offset.x, fbo.getHeight() / 2 + offset.y);
	fbo.end();

	fbo.readToPixels(pix); // the ofPixels class has a convenient getColor() method

	ofBackground(bg_color);

	bReset = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    if (doUpdateDrawMode)
    {
        drawMode = ++drawMode % 4; // move through 4 drawing modes (0, 1, 2, 3)
        ofBackground(bg_color); // clear the screen when changing drawing mode
        if(drawMode == 2){
            ofSetColor(255);
            fbo.draw(0, 0); // draw text to the screen for drawMode 2
        }
        bReset = true;
        doUpdateDrawMode = false;
    }
    
    if(bReset){
		resetParticles();
	}
	for(int i = 0; i < particles.size(); i++){
		particles[i]->update();
	}
    
}

//--------------------------------------------------------------
void ofApp::draw(){
	// display each particle in the list
	for(int i = 0; i < particles.size(); i++){
		particles[i]->display();
	}
}

//--------------------------------------------------------------
void ofApp::resetParticles(){
	bReset = false;
	particles.clear();
    for(int i = 0; i < particles.size(); i++){
        delete particles[i];
        particles[i]=NULL;
    }
	if(particles.size() < maxParticles)
    {
        int difference = maxParticles-particles.size();
        for (int i=0; i<difference; i++)
        {
            Particle* p = new Particle(&pix, fbo_color, drawMode);
            particles.push_back(p);
             ofLogVerbose() << particles.size() << " : " << i;
        }
       
		
	}
    
    ofLogVerbose() << particles.size() << " : " << maxParticles;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    doUpdateDrawMode = true;
	
}
