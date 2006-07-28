#include <il/il.h>
#include <il/ilu.h>
#include <il/ilut.h>


void main(int argc, char **argv)
{
    ILubyte *Lump;
    ILubyte *final_tga;
    ILubyte *Data;
    ILuint Size;
    ILuint Image;
    FILE *File;
    ILenum Error;
    ILuint ilFormat;
    int x0,y0,x1,y1;

    //char filename[]="103,692-205,768.tga";
    char filename[64];

    if(argc!=0 && !strcmp(argv[2],"-laststep"))
    {
		File=fopen(argv[1], "r+");
        fseek(File, 17, SEEK_SET);
        fprintf(File, "%c", 0x20);
        fclose(File);

	    ilInit();

		ilGenImages(1, &Image);
		ilBindImage(Image);

		if(!ilLoadImage(argv[1]))
		{
			printf("Error loading image final.tga");
			return ;
		}
	    ilEnable(IL_FILE_OVERWRITE);
		strcpy(filename, argv[1]);
		*strchr(filename, '\\')='\0';
		strcat(filename, "\\final.png");

		if(!ilSave(IL_PNG, filename))
			printf("Error occurs when saving image.");
		else
			printf("Image is saved successfuly.");


        return ;
    }
    strcpy(filename, strchr(argv[2], '\\')+1);
    sscanf(filename, "%d, %d - %d, %d", &x0,&y0,&x1,&y1);
    //printf("%d %d %d %d\n", x0,y0,x1,y1);
    x0--;	//start from (0,0), not (1,1)
    y0--;
    ilInit();

    File = fopen(argv[2], "rb");
    fseek(File, 0, SEEK_END);
    Size = ftell(File);
    fclose(File);

    Data = (ILubyte*)malloc(Size);

    ilGenImages(1, &Image);
    ilBindImage(Image);

    if(!ilLoadImage(argv[2]))
    {
        printf("Error loading image %s", filename);
        return ;
    }
    ilFormat=ilGetInteger(IL_IMAGE_FORMAT);
    ilCopyPixels(x0, y0, 0, x1-x0, y1-y0, 1, ilFormat, IL_UNSIGNED_BYTE, Data);
    if(!ilLoadImage(argv[1]))
    {
        printf("Error loading image final.tga");
        return ;
    }
    ilSetPixels(x0, y0, 0, x1-x0, y1-y0, 1, ilFormat, IL_UNSIGNED_BYTE, Data);
    ilEnable(IL_FILE_OVERWRITE);

    if(!ilSaveImage(argv[1]))
		printf("Error occurs when saving image.");
    else
        printf("Image is saved successfuly.");
    ilDeleteImages(1, &Image);

    ilShutDown();

    while ((Error = ilGetError()) != IL_NO_ERROR)
    {
        printf("%d: %s/n", Error, iluErrorString(Error));
    }
}
