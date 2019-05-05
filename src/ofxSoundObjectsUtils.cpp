//
//  ofxSoundObjectsUtils.cpp
//  example-soundPlayerObject
//
//  Created by Roy Macdonald on 25-11-17.
//
//

#include "ofxSoundObjectsUtils.h"

//--------------------------------------------------------------
void ofxSoundObjects::getBufferFromChannelGroup(const ofSoundBuffer & sourceBuffer, ofSoundBuffer & targetBuffer, std::vector<int> group){
	auto channels = sourceBuffer.getNumChannels();
	if(channels == 0) {
		ofLogWarning("ofxSoundBaseMultiplexer") << "getChannels requested on empty buffer";
		return;
	}
	ofRemove(group, [&](int & i){return i >= channels;});
	if(group.size() == 0){
		ofLogWarning("ofxSoundBaseMultiplexer") << "no valid group indices";
		return;
	}
	if (group.size() >= channels){
		ofLogNotice("ofxSoundBaseMultiplexer") << "getChannels requested more channels than available ";
	}
	
	//	targetBuffer.setNumChannels(group.size());
	targetBuffer.setSampleRate(sourceBuffer.getSampleRate());
	auto nFrames = sourceBuffer.getNumFrames();
	//	if(channels == 1){
	//		sourceBuffer.copyTo(targetBuffer, nFrames, 0, 0);
	//	}else{
	auto & buffer = sourceBuffer.getBuffer();
	targetBuffer.allocate(nFrames, group.size());
	for (std::size_t k = 0; k < group.size(); k++) {
		for(std::size_t i = 0; i < nFrames; i++){
			targetBuffer[k + i * group.size()] = buffer[group[k] + i * channels];
		}
	}
	//	}
}
//--------------------------------------------------------------
void ofxSoundObjects::setBufferFromChannelGroup(const ofSoundBuffer & sourceBuffer, ofSoundBuffer & targetBuffer, const std::vector<int>& group){
	//    auto channels = std::max(targetBuffer.getNumChannels(), (std::size_t)*std::max_element(group.begin(), group.end()));
	//    targetBuffer.setNumChannels(channels);
	//	targetBuffer.resize(sourceBuffer.getNumFrames() * channels);
	if(sourceBuffer.getSampleRate() != targetBuffer.getSampleRate()){
		ofLogWarning("ofxSoundBaseMultiplexer::setChannels") << "Different sample rates in buffers!! sourceBuffer: " <<sourceBuffer.getSampleRate() << "  targetBuffer: " << targetBuffer.getSampleRate();
	}
	auto nFrames = targetBuffer.getNumFrames();
	auto & buffer = targetBuffer.getBuffer();
	for (std::size_t k = 0; k < group.size(); k++) {
		for(std::size_t i = 0; i < nFrames; i++){
			buffer[group[k]+ i * targetBuffer.getNumChannels()] = sourceBuffer[k + i * group.size()];
		}
	}
}
//--------------------------------------------------------------
bool ofxSoundObjects::checkBuffers(const ofSoundBuffer& src, ofSoundBuffer& dst, bool bSetDst){
	if(dst.size()!=src.size()) {
		ofLogVerbose("ofxSoundObject") << "working buffer size != output buffer size. " << dst.size() << " != " <<src.size();
		if(bSetDst){
			dst.resize(src.size());
			dst.setNumChannels(src.getNumChannels());
			dst.setSampleRate(src.getSampleRate());
		}
		return true;
	}
	return false;
}
//--------------------------------------------------------------
std::vector<ofSoundDevice> getSoundDevices(bool bGetInputs){
	auto ll = ofGetLogLevel();//Ugly hack to avoid printing when calling ofSoundStreamListDevices()
	ofSetLogLevel(OF_LOG_SILENT);
	auto devices = ofSoundStreamListDevices();
	ofSetLogLevel(ll); 
	std::vector<ofSoundDevice> devs;
	for(auto& d: devices){
		if((bGetInputs && d.inputChannels > 0) || (!bGetInputs && d.outputChannels > 0)){
			devs.push_back(d);
		}
		
	}
	return devs;
}
//--------------------------------------------------------------
std::vector<ofSoundDevice> ofxSoundObjects::getInputSoundDevices(){
	return getSoundDevices(true);
}
//--------------------------------------------------------------
std::vector<ofSoundDevice> ofxSoundObjects::getOutputSoundDevices(){
	return getSoundDevices(false);
}
//--------------------------------------------------------------
std::string ofxSoundObjects::getSoundDeviceString(ofSoundDevice dev, bool bInputs, bool bOutputs){
	std::stringstream ss;
	ss << dev.name;
	if(bInputs && dev.isDefaultInput){ ss << " [default]";}
	else if(!bOutputs && dev.isDefaultOutput){ ss << " [default]";}
	ss  << std::endl;
	
	ss << "     API: " << toString(dev.api) << std::endl;
	ss << "     ID: " << dev.deviceID << std::endl;
	
	if(bInputs)  ss << "     Inputs :" << dev.inputChannels << std::endl; 
	if(bOutputs) ss << "     Outputs :" << dev.outputChannels << std::endl;
	
	return ss.str();

}
void printDevices(const std::string& msg, const std::vector<ofSoundDevice>& devs, bool bInputs){
	
	std::stringstream ss;
	ss  << std::endl;
	if(ofGetLogLevel() != OF_LOG_SILENT){
		ss << "------- SOUND "<< (bInputs?"INPUT":"OUTPUT") << " DEVICES ---------" << endl;
		for(size_t i = 0; i < devs.size(); i++){				
			auto& dev = devs[i]; 
			ss << "[ "<< i << " ] "  << ofxSoundObjects::getSoundDeviceString(devs[i], bInputs, !bInputs);			
		}
	}
	ofLogNotice(msg,ss.str());
}
//--------------------------------------------------------------
void  ofxSoundObjects::printInputSoundDevices(){
	printDevices("ofxSoundObjects::printInputSoundDevices", getInputSoundDevices(), true); 
}
//--------------------------------------------------------------
void  ofxSoundObjects::printOutputSoundDevices(){
	printDevices("ofxSoundObjects::printOutputSoundDevices", getOutputSoundDevices(), false);
}
//--------------------------------------------------------------
void ofxSoundObjects::getBufferPeaks(ofSoundBuffer& buffer, std::vector<float>& peaks){
	auto nc = buffer.getNumChannels();
	auto nf = buffer.getNumFrames();
	auto & b = buffer.getBuffer();
	if(peaks.size() != nc) peaks.resize(nc, 0.0f);
	size_t i;
	for(size_t c = 0; c < nc; c++){
		for(size_t f = 0; f < nf; f++){
			i = f * nc + c;
			if(peaks[c] < b[i]) peaks[c] = b[i];
		}
	}
	
}



