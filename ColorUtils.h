//---------------------------------------------------------------------------
#ifndef ColorUtilsH
#define ColorUtilsH

#include <stdint.h>
//---------------------------------------------------------------------------
/*! \file classKolor.h Plik zawierający klasę 'classKolor'.
*/
/*! \brief Klasa opisująca kolor wg modelu SVH.
*/
typedef struct HsvColor
{
	uint8_t h; //0-255
	uint8_t s; //0-255
	uint8_t v; //0-255
} HsvColor;
//---------------------------------------------------------------------------
/*! \brief Klasa opisująca kolor wg modelu RGB.
*/
typedef struct RgbColor
{
	uint8_t r; //0-255
	uint8_t g; //0-255
	uint8_t b; //0-255
} RgbColor;

//---------------------------------------------------------------------------

/*! \brief Główna klasa classKolor.

Umożliwia konwersję między przestrzeniami kolorów
*/

class ColorUtils
{
public:
	static RgbColor HsvToRgb(HsvColor hsv); //!< Funkcja statyczna konwertująca HSV -> RGB, wszystie wartości //0-255
	static HsvColor RgbToHsv(RgbColor rgb); //!< Funkcja statyczna konwertująca RGB -> HSV, wszystie wartości //0-255
};
#endif
