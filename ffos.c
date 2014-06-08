#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <jansson.h>
#include "ffos.h"

/**
 * responds to ffos create appname
 */
void create(int argc,char **argv) {
	if (argc!=3) {
		help();
		exit(1);
	}
	char *appname = argv[2];
	
	/* create the appname folder */
	mkdir(appname,S_IRWXU);
	
	chdir(appname);
	/* create subfolders for css, javascript and image files */
	mkdir("css",S_IRWXU);
	mkdir("js",S_IRWXU);
	mkdir("img",S_IRWXU);
	
	/* create index.html */
	FILE *index = fopen("index.html","w+");
	if (!index) {
		fprintf(stderr,"Error on creating %s\\index.html",appname);
		exit(2);
	}
	fprintf(index,"<!DOCTYPE html>\n");
	fprintf(index,"<html>\n");
	fprintf(index,"<head>\n<title>%s</title>\n</head>\n\n",appname);
	fprintf(index,"<body>\n<div>\n\tContent goes here\n</div>\n</body>\n\n");
	fprintf(index,"</html>");
	fclose(index);
	
	/* create manifest.webapp */
	FILE *mf = fopen("manifest.webapp","w+");
	if(!mf) {
		fprintf(stderr,"Error on creating %s\\manifest.webapp",appname);
		exit(3);
	}
	/* create json_t object to write the manifest */
	json_t *manifest = json_object();
	json_object_set(manifest,"name",json_string(appname));
	json_object_set(manifest,"launch_path",json_string("\\index.html"));
	json_object_set(manifest,"description",json_string("App description here"));	
	json_object_set(manifest,"version",json_string("1.0"));
	
	/* developer information */
	json_t *dev = json_object();
	json_object_set(dev,"name",json_string("Your name here"));
	json_object_set(dev,"url",json_string("Your URL here"));
	json_object_set(manifest,"developer",dev);
	
	json_dumpf(manifest,mf,JSON_ENSURE_ASCII | JSON_INDENT(4));
	fclose(mf);
	
	/* create ffosproj.info file */
	char *infofile="ffosproj.info";
	FILE *info = fopen(infofile,"w+b");
	if (!info) {
		fprintf(stderr,"Could not create %s\n",appname);
		exit(4);
	}
	fprintf(info,"App name: %s\n",appname);
	const time_t now = time(NULL);
	char *str_time = ctime(&now);
	fprintf(info,"Date: %s",str_time);
	fclose(info);
	
	printf("Project %s created successfully\n",appname);
}

void add(int argc,char **args) {
	if(argc<4) {
		help();
		exit(1);
	}
	
	int status = check();
	
	if(status == NOT_FFOS_PRO) {
		fprintf(stderr,"This folder is not a FFOS app\n");
		exit(2);
	}
	
	char *type = args[2];
	/* add css || js file */
	if (!strcmp(type,"css") || !strcmp(type,"js")) {
		//goto the correct folder
		chdir(type);
		char *filename = args[3];
		strcat(filename,".");
		strcat(filename,type);
		FILE *fp = fopen(filename,"w+");
		if (!fp) {
			fprintf(stderr,"Could not create %s\n",filename);
			exit(2);
		}
		fprintf(fp,"/**\n");
		fprintf(fp," * Author: Your Name Here\n");
		
		/* calculate the date */
		const time_t now = time(NULL);
		char *str_time = ctime(&now);
		fprintf(fp," * Date: %s",str_time);
		fprintf(fp," */\n");
		fclose(fp);
	}
	
	/* add html file */
	else if (!strcmp(type,"html")) {
		char *filename = args[3];
		strcat(filename,".html");
		FILE *fp = fopen(filename,"w+");
		if (!fp) {
			fprintf(stderr,"Could not create %s\n",filename);
			exit(2);
		}
		fclose(fp);
	}
	
}

void set(int argc,char **args) {
	if (check() == NOT_FFOS_PRO) {
		fprintf(stderr,"This folder is not a FFOS app\n");
		exit(1);
	}
	if(argc<4) {
		help();
		exit(2);
	}
	
	char *mfname = "manifest.webapp";
	
	json_t *manifest;
	json_error_t error;
	
	manifest = json_load_file(mfname,JSON_DISABLE_EOF_CHECK,&error);
	
	if (!manifest) {
		fprintf(stderr,"Could not parse manifest.webapp\n");
		fprintf(stderr,"line %d\n",error.line);
		fprintf(stderr,"message: %s\n",error.text);
		fprintf(stderr,"source: %s\n",error.source);
		exit(4);
	}
	
	char *whatset = args[2];
	
	if (!strcmp(whatset,"dev.name")) {
		char *dev_name = args[3];
		for (int i=4;i<argc;i++) {
			sprintf(dev_name,"%s %s",dev_name,args[i]);
		}
		
		json_t *dev = json_object_get(manifest,"developer");
		
		json_object_set(dev,"name",json_string(dev_name));
		json_object_set(manifest,"developer",dev);
		json_dump_file(manifest,mfname,JSON_INDENT(4));
	}
	
	else if (!strcmp(whatset,"dev.url")) {
		char *dev_url = args[3];
		json_t *dev = json_object_get(manifest,"developer");
		json_object_set(dev,"url",json_string(dev_url));
		json_object_set(manifest,"developer",dev);
		json_dump_file(manifest,mfname,JSON_INDENT(4));
	}
	
	else if (!strcmp(whatset,"launch_path")) {
		char *lpath = args[3];
		json_object_set(manifest,"launch_path",json_string(lpath));
		json_dump_file(manifest,mfname,JSON_INDENT(4));
	}
	
	else if (!strcmp(whatset,"version")) {
		char *ver = args[3];
		json_object_set(manifest,"version",json_string(ver));
		json_dump_file(manifest,mfname,JSON_INDENT(4));
	}
	
	else if (!strcmp(whatset,"descr")) {
		char *descr = args[3];
		json_object_set(manifest,"description",json_string(descr));
		json_dump_file(manifest,mfname,JSON_INDENT(4));
	}
	
	else if (!strcmp(whatset,"permission")) {
		if (argc != 5) {
			help();
			exit(3);
		}
		
		char *prm = args[3];
		char *descr = args[4];
		
		json_t *permissions = json_object_get(manifest,"permissions");
		if (!permissions) permissions = json_object();
		json_t *new_perm = json_object();
		json_object_set(new_perm,"description",json_string(descr));
		json_object_set(permissions,prm,new_perm);
		json_object_set(manifest,"permissions",permissions);
		json_dump_file(manifest,mfname,JSON_INDENT(4));
	}

}

void help() {
	printf("ffos-cli %s to help developing Firefox OS applications easier\n\n",FFOS_VERSION);
	printf("commands:\n");
	printf("    create - Creates a new Firefox OS application folder with necessary files\n");
	printf("    add - Adds new source files to the working project\n");
	printf("    set - Changes data stored in manifest file");
	printf("\n\n");
	printf("For more help check the README or type ffos help [command]\n\n");	
}

void cmd_help(int argc,char **args) {
	if (argc != 3) {
		help();
		exit(1);
	}
	char *cmd = args[2];
	if(!strcmp(cmd,"create")) {
		printf("    ffos create [appname] - creates an application with the specified name\n\n");
	}
	
	else if(!strcmp(cmd,"add")) {
		printf("    ffos add css [file] - creates css/file.css\n");
		printf("    ffos add js [file] - creates js/file.js\n");
		printf("    ffos add html [file] - creates file.html\n\n");
	}
	
	else if (!strcmp(cmd,"set")) {
		printf("    ffos set dev.name [dev name] - sets the developer's name to [dev name]\n");
		printf("    ffos set dev.url [dev url] - sets the developer's url to [dev url]\n");
		printf("    ffos set launch_path [lp] - sets the launch path to [lp]\n");
		printf("    ffos set version [ver] - sets the app version to [ver]\n");
		printf("    ffos set descr [description] - sets the description to [description]\n");
		printf("    ffos set permission <permission> [description] - sets a permission to the manifest.webapp file\n\n");
	}
	
	else help();
}

/**************** ffos helper functions ****************/

int check() {
	char *infofile="ffosproj.info";
	FILE *info = fopen(infofile,"rb");
	if (!info) {
		return NOT_FFOS_PRO;
	}
	fclose(info);
	return FFOS_PRO;
}
