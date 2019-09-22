//
//  waveformDraw.h
//  example-audio-sender
//
//  Created by Roy Macdonald on 9/17/19.
//
//

#pragma once

#pragma once
#include "ofMain.h"
#include "ofxSoundObject.h"
#include "ofxSoundUtils.h"
#include "ofx2DCanvas.h"

template<typename BufferType>
class waveformDraw_: public ofRectangle, public ofxSoundObject{
public:
	
	virtual  std::string getName() override{ return "waveForm";}
	
	waveformDraw_():ofxSoundObject(OFX_SOUND_OBJECT_PROCESSOR){
		bRenderWaveforms = false;
		bMakeGrid = false;
	}
	waveformDraw_(const waveformDraw_& a): ofRectangle(a), ofxSoundObject(a){}

	void setup(const ofRectangle& r);
    void setup(float x, float y, float w, float h);
    virtual void process(ofSoundBuffer &input, ofSoundBuffer &output) override;
	
	
	void draw(const ofRectangle& viewport = ofRectangle());
	
	// you can use this method to render and draw a static buffer.
	void makeMeshFromBuffer(const ofSoundBuffer& buffer);
	
	void setGridSpacingByNumSamples(size_t spacing);
	

protected:
	
	virtual void updateWaveformMesh();
	
	void makeWaveformMesh();
 
	void makeGrid();
	
	ofx2DCanvas canvas;
	
	vector<ofVboMesh>waveforms;
	
	ofVboMesh gridMesh;
	
	size_t gridSpacing = 0;

	
	bool bCanvasIsSetup = false;
	
	
	std::atomic<bool> bRenderWaveforms;
	std::atomic<bool> bMakeGrid;
	mutable ofMutex mutex1;
	
	BufferType buffer;

};

typedef waveformDraw_<ofSoundBuffer> waveformDraw;



class circularBufferWaveformDraw : public waveformDraw_<ofxCircularSoundBuffer>{
public:
	
	virtual void process(ofSoundBuffer &input, ofSoundBuffer &output) override;
	
	
	void setNumBuffers(size_t numBuffers){
		this->numBuffers = numBuffers;
	}

protected:
	virtual void updateWaveformMesh() override;
	
private:
	size_t numBuffers = 100;

};



