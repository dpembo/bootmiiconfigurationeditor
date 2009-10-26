#include <mxml.h>
#include <fat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include "../langvars.h"
#include "../skinvars.h"
#include "../version.h"
#include "../console/console.h"

#include "../wiimote/wiimote.h"
#include "../file/file.h"
#include "../stringutils/stringutils.h"


mxml_node_t *settingstree;
mxml_node_t *settingsdata;

mxml_node_t *langstree;
mxml_node_t *langsdata;
mxml_node_t *langsnode;

mxml_node_t *skinstree;
mxml_node_t *skinsdata;
mxml_node_t *skinsnode;




char* getFullFileName(char* fileName)
{
	return strdup(concat(strdup(app_path),fileName));
}

void update_settings() 
{
	//mxml_node_t *settingstree;
	//mxml_node_t *settingsdata;
	
	
	
	FILE *fp = fopen(getFullFileName("bce_settings.xml"), "rb");
	//printf("About to load XML\n\n");

	if (fp == NULL) 
	{
		//printf("ERROR!");
		//press1ToContinue();
		fclose(fp);
		drawError(15,11,11,50,"Unable to open settings file [bce_settings.xml]");
		
	}
	else 
	{
		fseek (fp , 0, SEEK_END);
		long settings_size = ftell (fp);
		rewind (fp);
		
		if (settings_size > 0) 
		{
			
			settingstree = mxmlLoadFile(NULL, fp, MXML_NO_CALLBACK);
			fclose(fp);
			
			selected_skin = strdup(skins_code[selSkinOption]);
			selected_language = strdup(languages_code[selLangOption]);
			
			settingsdata = mxmlFindElement(settingstree, settingstree, "selected", NULL, NULL, MXML_DESCEND);
			mxmlElementSetAttr(settingsdata,"code",strdup(languages_code[selLangOption]));
			mxmlElementSetAttr(settingsdata,"skin",strdup(skins_code[selSkinOption]));
			
			settingsdata = mxmlFindElement(settingstree, settingstree, "update", NULL, NULL, MXML_DESCEND);
			
			if(checkNewVersion==true)
			{
				mxmlElementSetAttr(settingsdata,"check","1");
			}
			else
			{
				mxmlElementSetAttr(settingsdata,"check","0");
			}
			
			
			FILE *f;
			f = fopen(getFullFileName("bce_settings.xml"), "wb");	
			
			if (f == NULL) 
			{
				fclose(f);
				//printf("Settings could not be written.\n");
				//press1ToContinue();
				drawError(15,11,11,50,"Unable to write settings to [bce_settings.xml]");
			}
			else 
			{
				mxmlSaveFile(settingstree, f, MXML_NO_CALLBACK);
				fclose(f);
				mxmlDelete(settingsdata);
				mxmlDelete(settingstree);
				//printf("Settings Saved\n");
			}
			
			
		}
	}
	
	fp = NULL;
	settingsdata = NULL;
	settingstree = NULL;
}

void parseUpdateXml(char* xml)
{
	/*mxml_node_t *tree;
	mxml_node_t *data;
	tree = mxmlLoadString(NULL,xml,MXML_NO_CALLBACK);
	
	data = mxmlFindElement(tree, tree, "major", NULL, NULL, MXML_DESCEND);
	major_version= atoi(mxmlElementGetAttr(data,"version"));
	
	data = mxmlFindElement(tree, tree, "minor", NULL, NULL, MXML_DESCEND);
	minor_version= atoi(mxmlElementGetAttr(data,"version"));	
	
	data = mxmlFindElement(tree, tree, "release", NULL, NULL, MXML_DESCEND);
	updateDescription= strdup(mxmlElementGetAttr(data,"description"));	*/
	
	//mxmlDelete(data);
	//mxmlDelete(tree);
	
}

void load_settingsFile()
{

	//mxml_node_t *settingstree;
	//mxml_node_t *settingsdata;
	
	FILE *fp = fopen(getFullFileName("bce_settings.xml"), "rb");
	//printf(" Loading Settings XML....... ");

	if (fp == NULL) 
	{
	
		fclose(fp);
		drawError(15,11,11,50,"Unable to open language file [bce_settings.xml]");
	}
	else 
	{
	
fseek (fp , 0, SEEK_END);
		long settings_size = ftell (fp);
		rewind (fp);
		
		if (settings_size > 0) 
		{
			
			settingstree = mxmlLoadFile(NULL, fp, MXML_NO_CALLBACK);
			fclose(fp);
			
			//data = mxmlFindElement(tree, tree, "settings", NULL, NULL, MXML_DESCEND);
			
			//Language Choices
			//data = mxmlFindElement(tree, tree, "available", NULL, NULL, MXML_DESCEND);
			//  mxml_node_t *node;
			//sleep(5);
			//printf("STARTING LANGUAGES\n");
			
			
			settingsdata = mxmlFindElement(settingstree, settingstree, "update", NULL, NULL, MXML_DESCEND);
			updateCheckURL = strdup(mxmlElementGetAttr(settingsdata,"url"));
			//downloadURL = strdup(mxmlElementGetAttr(settingsdata,"downloadurl"));
			
			downloadURL = strdup(updateCheckURL);
			
			int checkval;
			checkval = atoi(mxmlElementGetAttr(settingsdata,"check"));
			if(checkval==1)
			{
				checkNewVersion = true;
			}
			else
			{
				checkNewVersion = false;
			}
			
			
			//Language Strings
			settingsdata = mxmlFindElement(settingstree, settingstree, "selected", NULL, NULL, MXML_DESCEND);
			selected_language = strdup(mxmlElementGetAttr(settingsdata,"code"));
			selected_skin = strdup(mxmlElementGetAttr(settingsdata,"skin"));	

			mxmlDelete(settingsdata);
			mxmlDelete(settingstree);
			
			//printf(" Done\n\n");
			//press1ToContinue();
		}
		else 
		{
			fclose(fp);
			//unlink("sd:/apps/bootmii/bce_messages.xml");
			drawError(15,11,11,50,"Unable to open settings file [bce_settings.xml]");
		}		
	
	}

}

void parseSkinsFile(char* selSkin)
{
	
	//sleep(5);
	
	skinsdata = mxmlFindElement(skinstree, skinstree, "skindetail", "id", selSkin, MXML_DESCEND);
	skin_console_bgcolor   = atoi(mxmlElementGetAttr(skinsdata,"consolebgcolor"));
	//printf("skin_console_bgcolor [%i]\n",skin_console_bgcolor);
	skin_console_textcolor = atoi(mxmlElementGetAttr(skinsdata,"consoletextcolor"));
	//printf("skin_console_bgcolor [%i]\n",skin_console_textcolor);
	
	//sleep(2);
	skin_main_bgcolor   = atoi(mxmlElementGetAttr(skinsdata,"mainbgcolor"));
	skin_main_textcolor = atoi(mxmlElementGetAttr(skinsdata,"maintextcolor"));
	skin_main_textcolor2   = atoi(mxmlElementGetAttr(skinsdata,"maintextcolor2"));
	skin_main_titlecolor = atoi(mxmlElementGetAttr(skinsdata,"maintitlecolor"));
	
	//printf("maintitlecolor [%i]\n",skin_main_titlecolor);
	//sleep(2);
	//printf("maintitlelinecolor [%i]\n,",atoi(mxmlElementGetAttr(data,"maintitlelinecolor")));
	//sleep(2);
	skin_main_titlelinecolor = atoi(mxmlElementGetAttr(skinsdata,"maintitlelinecolor"));
	//printf("DONE\n");
	//sleep(2);
	
	//printf("skin_main_titlelinecolor [%i]\n",skin_main_titlelinecolor);
	
	//sleep(2);
	skin_main_selectedIndicatorcolor   = atoi(mxmlElementGetAttr(skinsdata,"mainselectedIndicatorcolor"));
	skin_main_selectedOptioncolor = atoi(mxmlElementGetAttr(skinsdata,"mainselectedOptioncolor"));
	skin_main_shadow = atoi(mxmlElementGetAttr(skinsdata,"mainshadow"));
	//printf("done skin_main");
	//sleep(2);
	skin_dialog_bgcolor   = atoi(mxmlElementGetAttr(skinsdata,"dialogbgcolor"));
	skin_dialog_textcolor = atoi(mxmlElementGetAttr(skinsdata,"dialogtextcolor"));
	skin_dialog_textcolor2   = atoi(mxmlElementGetAttr(skinsdata,"dialogtextcolor2"));
	skin_dialog_titlecolor = atoi(mxmlElementGetAttr(skinsdata,"dialogtitlecolor"));
	skin_dialog_titlelinecolor = atoi(mxmlElementGetAttr(skinsdata,"dialogtitlelinecolor"));
	skin_dialog_selectedIndicatorcolor   = atoi(mxmlElementGetAttr(skinsdata,"dialogselectedIndicatorcolor"));
	skin_dialog_selectedOptioncolor = atoi(mxmlElementGetAttr(skinsdata,"dialogselectedOptioncolor"));
	skin_dialog_shadow = atoi(mxmlElementGetAttr(skinsdata,"dialogshadow"));			
	skin_dialog_shadow = atoi(mxmlElementGetAttr(skinsdata,"dialogshadow"));			
	
	skin_main_decl  = atoi(mxmlElementGetAttr(skinsdata,"maindecl"));			
	skin_dialog_decl= atoi(mxmlElementGetAttr(skinsdata,"dialogdecl"));			
	
	//mxmlDelete(skinsnode);
	//mxmlDelete(skinsdata);
	//mxmlDelete(skinstree);
}

void load_xmlSkinsFile()
{
	//mxml_node_t *tree;
	//mxml_node_t *data;
	//mxml_node_t *node;
	
	FILE *fp = fopen(getFullFileName("bce_skins.xml"), "rb");
	//printf(" Loading Settings XML....... ");

	int i=0;
	for (i = 0;i<10;i++)
	{
		skins_desc[i]="*NA*";
		skins_code[i]="*NA*";
	}
			
	if (fp == NULL) 
	{
	
		fclose(fp);
		drawError(15,11,11,50,"Unable to open skins file [bce_skins.xml]");
	}
	else
	{
		fseek (fp , 0, SEEK_END);
		long settings_size = ftell (fp);
		rewind (fp);
		
		if (settings_size > 0) 
		{
			
			skinstree = mxmlLoadFile(NULL, fp, MXML_NO_CALLBACK);
			fclose(fp);	
			
			int skinCount = 0;
			for (skinsnode = mxmlFindElement(skinstree, skinstree,"skindetail",NULL, NULL,MXML_DESCEND);skinsnode != NULL;skinsnode = mxmlFindElement(skinsnode, skinstree,"skindetail",NULL, NULL,MXML_DESCEND))
			{
				skins_code[skinCount]=strdup(mxmlElementGetAttr(skinsnode,"id"));
				skins_desc[skinCount]=strdup(mxmlElementGetAttr(skinsnode,"name"));
				if(strcmp(skins_code[skinCount],selected_skin)==0)selSkinOption=(skinCount+0);
				skinCount++;
				//printf("Found skin [%s]-[%s]\n",strdup(mxmlElementGetAttr(skinsnode,"id")),strdup(mxmlElementGetAttr(skinsnode,"name")));
			}
			numberSkins=skinCount;
			
			parseSkinsFile(strdup(selected_skin));
		}
		else 
		{
			fclose(fp);
			//unlink("sd:/apps/bootmii/bce_messages.xml");
			drawError(15,11,11,50,"Unable to open skins file [bce_skins.xml]");
		}		
	}
}

void parseLanguageFile(char* selLangCode)
{
	
	//for (i = 0;i<10;i++)
	//{
	//	printf("[%i] [%s]-[%s]",i,languages_desc[i],languages_code[i]);
		//skins_desc[count]="*NA*";
		//skins_code[count]="*NA*";
	//}
	
	  
	
	langsdata = mxmlFindElement(langstree, langstree, "entry", "code", "mm1", MXML_DESCEND);
	mm1 = strdup(mxmlElementGetAttr(langsdata,selLangCode));
	
	langsdata = mxmlFindElement(langstree, langstree, "entry", "code", "mm2", MXML_DESCEND);
	mm2 = strdup(mxmlElementGetAttr(langsdata,selLangCode));

	langsdata = mxmlFindElement(langstree, langstree, "entry", "code", "mm3", MXML_DESCEND);
	mm3 = strdup(mxmlElementGetAttr(langsdata,selLangCode));
	
	langsdata = mxmlFindElement(langstree, langstree, "entry", "code", "mm4", MXML_DESCEND);
	mm4 = strdup(mxmlElementGetAttr(langsdata,selLangCode));

	langsdata = mxmlFindElement(langstree, langstree, "entry", "code", "mm5", MXML_DESCEND);
	mm5 = strdup(mxmlElementGetAttr(langsdata,selected_language));
	
	langsdata = mxmlFindElement(langstree, langstree, "entry", "code", "mm6", MXML_DESCEND);
	mm6 = strdup(mxmlElementGetAttr(langsdata,selLangCode));			
	
	langsdata = mxmlFindElement(langstree, langstree, "entry", "code", "up1", MXML_DESCEND);
	up1 = strdup(mxmlElementGetAttr(langsdata,selLangCode));				
	
	langsdata = mxmlFindElement(langstree, langstree, "entry", "code", "up2", MXML_DESCEND);
	up2 = strdup(mxmlElementGetAttr(langsdata,selLangCode));							
	
	langsdata = mxmlFindElement(langstree, langstree, "entry", "code", "error", MXML_DESCEND);
	error = strdup(mxmlElementGetAttr(langsdata,selLangCode));		

	langsdata = mxmlFindElement(langstree, langstree, "entry", "code", "err1_initFatErr", MXML_DESCEND);
	err1_initFatErr = strdup(mxmlElementGetAttr(langsdata,selLangCode));	
	
	langsdata = mxmlFindElement(langstree, langstree, "entry", "code", "err2_rootFS", MXML_DESCEND);
	err2_rootFS = strdup(mxmlElementGetAttr(langsdata,selLangCode));	

	langsdata = mxmlFindElement(langstree, langstree, "entry", "code", "err3_noWrite", MXML_DESCEND);
	err3_noWrite = strdup(mxmlElementGetAttr(langsdata,selLangCode));	
	
	langsdata = mxmlFindElement(langstree, langstree, "entry", "code", "err4_noExist", MXML_DESCEND);
	err4_noExist = strdup(mxmlElementGetAttr(langsdata,selLangCode));				
	
	
	
	//printf("DONE LANGUAGES - starting skins");
	
	//printf("GETTING SKINS LIST\n");
	
	// SKIN SETTINGS
	//langsdata = mxmlFindElement(langstree, langstree, "selected", NULL, NULL, MXML_DESCEND);
	//selected_skin = strdup(mxmlElementGetAttr(data,"skin"));			

	
	//Now the selection options
	
	//Video Mode
	langsdata = mxmlFindElement(langstree, langstree, "entry", "code", "video_ntsc", MXML_DESCEND);
	videoOptionsDesc[0] = strdup(mxmlElementGetAttr(langsdata,selLangCode));
	langsdata = mxmlFindElement(langstree, langstree, "entry", "code", "video_pal50", MXML_DESCEND);
	videoOptionsDesc[1] = strdup(mxmlElementGetAttr(langsdata,selLangCode));
	langsdata = mxmlFindElement(langstree, langstree, "entry", "code", "video_pal60", MXML_DESCEND);
	videoOptionsDesc[2] = strdup(mxmlElementGetAttr(langsdata,selLangCode));
	langsdata = mxmlFindElement(langstree, langstree, "entry", "code", "video_prog", MXML_DESCEND);
	videoOptionsDesc[3] = strdup(mxmlElementGetAttr(langsdata,selLangCode));
	
	//Autoboot
	langsdata = mxmlFindElement(langstree, langstree, "entry", "code", "autoboot_off", MXML_DESCEND);
	autobootOptionsDesc[0] = strdup(mxmlElementGetAttr(langsdata,selLangCode));
	langsdata = mxmlFindElement(langstree, langstree, "entry", "code", "autoboot_sysm", MXML_DESCEND);
	autobootOptionsDesc[1] = strdup(mxmlElementGetAttr(langsdata,selLangCode));
	langsdata = mxmlFindElement(langstree, langstree, "entry", "code", "autoboot_hbc", MXML_DESCEND);
	autobootOptionsDesc[2] = strdup(mxmlElementGetAttr(langsdata,selLangCode));



	//NOW THE SETTINGS MENU
	
	langsdata = mxmlFindElement(langstree, langstree, "entry", "code", "setTitle", MXML_DESCEND);
	setTitle = strdup(mxmlElementGetAttr(langsdata,selLangCode));
	langsdata = mxmlFindElement(langstree, langstree, "entry", "code", "setLang", MXML_DESCEND);
	setLang = strdup(mxmlElementGetAttr(langsdata,selLangCode));
	langsdata = mxmlFindElement(langstree, langstree, "entry", "code", "setSkin", MXML_DESCEND);
	setSkin = strdup(mxmlElementGetAttr(langsdata,selLangCode));
	
	langsdata = mxmlFindElement(langstree, langstree, "entry", "code", "setUpdate", MXML_DESCEND);
	setUpdate = strdup(mxmlElementGetAttr(langsdata,selLangCode));
	
	langsdata = mxmlFindElement(langstree, langstree, "entry", "code", "setSave", MXML_DESCEND);
	setSave = strdup(mxmlElementGetAttr(langsdata,selLangCode));
	langsdata = mxmlFindElement(langstree, langstree, "entry", "code", "setCancel", MXML_DESCEND);
	setCancel = strdup(mxmlElementGetAttr(langsdata,selLangCode));
	
	langsdata = mxmlFindElement(langstree, langstree, "entry", "code", "enabled", MXML_DESCEND);
	enabled = strdup(mxmlElementGetAttr(langsdata,selLangCode));

	langsdata = mxmlFindElement(langstree, langstree, "entry", "code", "disabled", MXML_DESCEND);
	disabled = strdup(mxmlElementGetAttr(langsdata,selLangCode));
	
	updateOptionsDesc[0]=strdup(disabled);
	updateOptionsDesc[1]=strdup(enabled);
			

			//mxmlDelete(langsnode);
			//mxmlDelete(langsdata);
			//mxmlDelete(langstree);
			//langstree = NULL;
}
void load_xmlLanguageFile() 
{
	//mxml_node_t *tree;
	//mxml_node_t *data;
	
	//Load File
	//printf("\nAbout to load lang\n");
	//sleep(2);
	if(langstree == NULL)
	{
		//printf("langs tree was null - opening file\n");
		//sleep(2);
		FILE *fp = fopen(getFullFileName("bce_messages.xml"), "rb");
		//printf(" Loading Settings XML....... ");

		if (fp == NULL) 
		{
			fclose(fp);
			drawError(15,11,11,50,"Unable to open language file [bce_messages.xml]");
		}
		else 
		{
			fseek (fp , 0, SEEK_END);
			long settings_size = ftell (fp);
			rewind (fp);
		
			if (settings_size > 0) 
			{
			
				//printf("about to populate the xml stuff!\n");
				//sleep(2);
				langstree = mxmlLoadFile(NULL, fp, MXML_NO_CALLBACK);
				fclose(fp);
			}
			else 
			{
				fclose(fp);
				//unlink("sd:/apps/bootmii/bce_messages.xml");
				drawError(15,11,11,50,"Unable to open language file [bce_messages.xml]");
			}			
		}
	
	}
	//printf("Done load - getting values out!\n");
	//sleep(2);
	
	//FILE *fp = fopen("bce_messages.xml", "rb");
	//printf(" Loading Settings XML....... ");

	/*if (fp == NULL) 
	{
	
		fclose(fp);
		drawError(15,11,11,50,"Unable to open language file [bce_messages.xml]");
	}
	else 
	{
		fseek (fp , 0, SEEK_END);
		long settings_size = ftell (fp);
		rewind (fp);
		
		if (settings_size > 0) 
		{
			
			langstree = mxmlLoadFile(NULL, fp, MXML_NO_CALLBACK);
			fclose(fp);*/
			
			//data = mxmlFindElement(tree, tree, "settings", NULL, NULL, MXML_DESCEND);
			
			//Language Choices
			//data = mxmlFindElement(tree, tree, "available", NULL, NULL, MXML_DESCEND);
			 //mxml_node_t *node;
			//sleep(5);
			//printf("STARTING LANGUAGES\n");
			
			
			/*data = mxmlFindElement(tree, tree, "update", NULL, NULL, MXML_DESCEND);
			updateCheckURL = strdup(mxmlElementGetAttr(data,"url"));
			
			data = mxmlFindElement(tree, tree, "update", NULL, NULL, MXML_DESCEND);
			int checkval;
			checkval = atoi(mxmlElementGetAttr(data,"check"));
			if(checkval==1)
			{
				checkNewVersion = true;
			}
			else
			{
				checkNewVersion = false;
			}*/
			
			
			//Language Strings
			//data = mxmlFindElement(tree, tree, "selected", NULL, NULL, MXML_DESCEND);
			//selected_language = strdup(mxmlElementGetAttr(data,"code"));
			
			
			
			//printf("GETTING LANGUAGES LIST\n");
			
			int i=0;
			for (i = 0;i<10;i++)
			{
				languages_desc[i]="*NA*";
				languages_code[i]="*NA*";
				//skins_desc[count]="*NA*";
				//skins_code[count]="*NA*";
			}
			
			int count = 0;
			for (langsnode = mxmlFindElement(langstree, langstree,"language",NULL, NULL,MXML_DESCEND);langsnode != NULL;langsnode = mxmlFindElement(langsnode, langstree,"language",NULL, NULL,MXML_DESCEND))
			{
				languages_desc[count]=strdup(mxmlElementGetAttr(langsnode,"description"));
				languages_code[count]=strdup(mxmlElementGetAttr(langsnode,"code"));
				if(strcmp(languages_code[count],selected_language)==0)selLangOption=(count+0);
				//printf("Found lang [%i] [%s]-[%s]\n",count,strdup(mxmlElementGetAttr(langsnode,"code")),strdup(mxmlElementGetAttr(langsnode,"description")));
				count++;
			}
			//sleep(5);
			numberLangs=count;
			
			parseLanguageFile(strdup(selected_language));
			
			//printf(" Done\n\n");
			//press1ToContinue();
		//}
		//else 
		//{
		//	fclose(fp);
		//	//unlink("sd:/apps/bootmii/bce_messages.xml");
		//	drawError(15,11,11,50,"Unable to open language file [bce_messages.xml]");
		//}
	//}
}
