/**
 * @file Image2ASCIIcolors.h
 * @brief Header file for image2ascii
 */

#ifndef IMAGE2ASCIICOLORS_H_INCLUDED
#define IMAGE2ASCIICOLORS_H_INCLUDED

#include <vector>

/*-----------------------SETTINGS-----------------------------*/
//#define ChangeBlackWhite
//#define ExtendedASCIICharSet
/*------------------------------------------------------------*/
#define FontSize_X 8
#define FontSize_Y 8
#define CHAR_X 8
#define CHAR_Y 8
#ifdef ExtendedASCIICharSet
#define CHAR_NUM 255 /* Extended ASCII */
#else
#define CHAR_NUM 95  /* Standard ASCII */
#endif /* ExtendedASCIICharSet */
/**
 * @def ChangeBlackWhite
 *   @brief Define this if you want to set the background color to white (instead of black)
 * @def ExtendedASCIICharSet
 *   @brief Define this to enable extended ASCII support
 * @def FontSize_X
 *   @brief Width of a character in Console Window
 * @def FontSize_Y
 *   @brief Height of a character in Console Window
 * @def CHAR_X
 *   @brief Width  of one ASCII Char
 * @def CHAR_Y
 *   @brief Height of one ASCII Char
 * @def CHAR_NUM
 *   @brief Number of ASCII Characters to choose from
**/

/** @brief ASCII character number, character lines, character columns */
typedef struct
{
    char ARR[CHAR_NUM][CHAR_Y][CHAR_X]; /**< The matrix for each ASCII CHAR */
    unsigned char CHAR[CHAR_NUM];                /**< Corresponding ASCII CHAR */
    int WEIGHT[CHAR_NUM];               /**< The weight (brightness) value of each ASCII CHAR */
    int SCALING_WEIGHT[CHAR_NUM];       /**< The scaled weight value for each ASCII CHAR @see CalculateWeights() */
} CHAR_SET;

/** @brief The struct for (.png .ppm) Images */
typedef struct
{
    unsigned char *Image;       /**< the raw pixels for images */
    unsigned char *ASCII_Image; /**< the processed ascii image (contains characters) */
    //TCODColor *ASCII_Color;     /**< ascii image colors */
    unsigned Width;     /**< width of raw images in pixels */
    unsigned Height;    /**< heigth of raw images in pixels */
    unsigned WidthTile; /**< width of raw images in characters */
    unsigned HeightTile;/**< height of raw images in characters */
} IMAGE;

/** @brief The struct for (.png .ppm) Images */
typedef struct
{
	std::vector<unsigned char> ASCII_Image; /**< the processed ascii image (contains characters) */
	//TCODColor *ASCII_Color;     /**< ascii image colors */
	unsigned WidthTile; /**< width of raw images in characters */
	unsigned HeightTile;/**< height of raw images in characters */
} ASCII_IMAGE;

/** @brief SIZES struct */
typedef struct
{
    int X; /**< X: width */
    int Y; /**< Y: Height */
} SIZES;
/** @brief POINTS struct */
typedef struct
{
	double X; /**< X: coord */
	double Y; /**< Y: coord */
} POINTS;
/** @brief the size of one subsection in IMAGE which will be converted to chars */
typedef struct
{
    double width;
    double height;
    double scale; /**< a scale for default font FontSize_X FontSize_Y to retain proportion and used for keeping IMAGE on screen @see CalculateImageSizes() */
} SUBSECTION;
/** @brief CONSOLEINFO struct */
typedef struct
{
    SIZES FontSize;   /**< Console window font sizes */
    SIZES CharAmount; /**< Console window char amount per rows and columns */
    SIZES Size;       /**< Console window size in pixels */
} CONSOLEINFO;

void CharSetImporter(CHAR_SET*CharSet,const char* filename);
void PreciseProcessPNG(IMAGE* PNG,SUBSECTION& subsec,CHAR_SET& CharSet);
void CalculateWeights(CHAR_SET*CharSet);
void Indexer(CHAR_SET* CharSet,int char_index);
void WriteOutPic(IMAGE* PNG,SIZES TopLeft,SIZES BoxSize);
void CalculatePNGSizes(IMAGE* PNG,SUBSECTION* subsec,CONSOLEINFO Con);


#endif // IMAGE2ASCIICOLORS_H_INCLUDED
