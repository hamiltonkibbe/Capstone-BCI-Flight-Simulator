/**********************************************************************

    DataLogger.cpp
    Record EEG data to file

 Copyright (C) 2011 Hamilton Kibbe
 This code is made available under the MIT License.
 <http://www.opensource.org/licenses/mit-license.html>

**********************************************************************/
#include "DataLogger.h"




using namespace std;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	DataLogger::DataLogger
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DataLogger::DataLogger()
{

	source = NULL;
	sysParams = NULL;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	DataLogger::~DataLogger
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DataLogger::~DataLogger()
{
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	DataLogger::getIterationsInDuration
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int DataLogger::getIterationsInDuration(double duration)
{
	return ceil(duration/(((double)source->getBlockSize())/source->getSamplingRate()));
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	DataLogger::runTest
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void DataLogger::runTest()
{
	// Bring globals into scope
	extern GladeXML* gui;

	// Local Variables
	char notify_text[50];
	ChannelData* channelData;
	int nChannels = source->getNumberOfChannels();
	int channelSize = sysParams->getBlockSize();
	int iterations = getIterationsInDuration(test->getDuration());
	ofstream file(test->getFileName().c_str(),ofstream::binary);


    gdk_threads_enter();    // Get GDK lock

	// Get GUI handles
	GtkWidget *button = glade_xml_get_widget(gui, "close_log_button");
	GtkWidget *progress_bar = glade_xml_get_widget(gui, "log_progress_bar");
	GtkWidget *label = glade_xml_get_widget(gui, "log_notify");

	// Reset progress window
	gtk_widget_set_sensitive(button,FALSE);
	gtk_label_set_text(GTK_LABEL(label),"");
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar),0.0);

    gdk_threads_leave();    // Release GDK lock


	//Write the file header....
	file << "Test: " << test->getTestName();
	file << "   |   Filename: " << test->getFileName() << endl;
	file << "Sample Rate: " << sysParams->getSamplingRate() << endl;
	file << "Block Size: " << sysParams->getBlockSize() << endl;
	file << "# Of Channels: " << nChannels << endl;
	file << endl;
	file << endl;


	//iteratre through data blocks
	for (int j=0;j<iterations;j++)
	{
		channelData = source->getData();

		//iteratre through sample #s
		for(int afi=0;afi<channelSize;afi++)
		{
			//iterate through channels at each sample #
			for(int chanIdx=0; chanIdx<nChannels; ++chanIdx)
			{
				float* sigData = (float*)channelData->getDataForChannel(chanIdx+1);
				file << sigData[afi] << ",";
			}
			file << endl;
		}

		// update progress window
		gdk_threads_enter();    // Get GDK lock
		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar),(1.0*j/iterations));
		gdk_threads_leave();    // Release GDK lock
	}
	file.close();

	// make close button clickable & finish progress
	sprintf(notify_text, "Complete. Data written to %s", test->getFileName().c_str());
	gdk_threads_enter();    // Get GDK lock
	gtk_label_set_text(GTK_LABEL(label),notify_text);
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress_bar),1.0);
	gtk_widget_set_sensitive(button,TRUE);
	gdk_threads_leave();    // Release GDK lock
}



