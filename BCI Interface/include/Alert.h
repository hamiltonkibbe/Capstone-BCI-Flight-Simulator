#ifndef ALERT_H_
#define ALERT_H_

#include <cstdlib>
#include <glade/glade.h>


// THESE MUST ONLY BE
// CALLED FROM WITHIN
// THE GDK LOCK
void alert(const char* message);
void alert(const char* title, const char* message);
void status(const char* status);
int status_remove_timeout(void* data);
void say(const char* string);
#endif
