#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../headers/config.h"
#include "../headers/control.h"
#include "../headers/definitions.h"
#include "../headers/globals.h"
#include "../headers/input.h"
#include "../headers/logic.h"
#include "../headers/screen.h"
#include "../headers/system_logic.h"

void initializeGlobals() {
	running=1;
	currentSectionNumber=0;
	gamesInPage=0;
	CURRENT_SECTION.totalPages=0;
	favoritesSectionNumber=0;
	favoritesSize=0;
	currentCPU=OC_NO;
	favoritesSectionSelected=0;
	favoritesChanged=0;
	pictureMode=0;
	#ifndef TARGET_PC
	#endif
	srand(time(0));
}

void resetFrameBuffer () {
	system("/usr/sbin/unlockvt > /dev/null");
//	system("/usr/bin/reset");
//	system("echo 0 > /sys/devices/virtual/vtconsole/vtcon1/bind");
}

int main(int argc, char* argv[]) {
	#ifndef TARGET_PC
	resetFrameBuffer();
	#endif
	createConfigFilesInHomeIfTheyDontExist();
	loadConfig();
	initializeGlobals();
//	HW_Init();
	setupDisplay();
	int sectionCount=loadSections();
	loadFavorites();
	if (argv[1]!=NULL) {
		setSectionsState(argv[1]);
		currentSectionNumber=atoi(argv[2]);
		returnTo=atoi(argv[3]);
		pictureMode=atoi(argv[4]);
	} else {
		loadLastState();
	}
	#ifndef TARGET_PC
	initSuspendTimer();
	#endif
	determineStartingScreen(sectionCount);
	updateScreen();
	enableKeyRepeat();
	while (running) {
		while(pollEvent()){
			if(getEventType()==getKeyDown()){
				if (!isSuspended) {
					performAction();
					updateScreen();
				}
				#ifndef TARGET_PC
				resetTimeoutTimer();
				#endif
			} else if (getEventType()==getKeyUp()) {
				if(getPressedKey()==BTN_B) {
					hotKeyPressed=0;
					leftOrRightPressed=0;
					updateScreen();
				}
			}
		}
	}
	quit();
}
