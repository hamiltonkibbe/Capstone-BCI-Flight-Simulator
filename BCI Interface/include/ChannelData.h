/*
 ================================================================
 
 Abstract Channel Data Provider
 MindSonde / The Myelin Project
 
 Copyright (C) 2010 Saumitro Dasgupta.
 This code is made available under the MIT License.
 <http://www.opensource.org/licenses/mit-license.html>
 
 ================================================================
 */

#ifndef _CHANNELDATA_H
#define	_CHANNELDATA_H



class ChannelData {
	
public:
	
	virtual const void* getRawData() const = 0;
	virtual unsigned getNumberOfChannels() const = 0;
	virtual unsigned getSamplesPerChannel() const = 0;
	
	//The channel numbers are one-based
	virtual const void* getDataForChannel(unsigned int channel) = 0;
	
	virtual void setRawData(void* argData)=0;
	virtual void setNumberOfChannels(unsigned int nChannels)=0;
	virtual void setSamplesPerChannel(unsigned int nSamples)=0;
	
	virtual void updateData()=0;	
	
};



#endif
