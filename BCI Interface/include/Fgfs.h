#ifndef FGFS_H_
#define FGFS_H_


#include <cstdlib>
#include <glade/glade.h>
#include <string>
#include "Instrument.h"
#include "Socket.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// FGFS interface functions
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void update_fgfs_display(std::string latitude,std::string longitude,std::string speed,std::string heading,
                         std::string altitude,std::string pitch,std::string roll );

std::string getData(std::string s);


#endif
