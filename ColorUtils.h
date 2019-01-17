//---------------------------------------------------------------------------
#ifndef ColorUtilsH
#define ColorUtilsH

#include <stdint.h>
//---------------------------------------------------------------------------
/*! \file classKolor.h Plik zawieraj¹cy klasê 'classKolor'.
*/
/*! \brief Klasa opisuj¹ca kolor wg modelu SVH.
*/
typedef struct HsvColor
{
	uint8_t h; //0-255
	uint8_t s; //0-255
	uint8_t v; //0-255
} HsvColor;
//---------------------------------------------------------------------------
/*! \brief Klasa opisuj¹ca kolor wg modelu RGB.
*/
typedef struct RgbColor
{
	uint8_t r; //0-255
	uint8_t g; //0-255
	uint8_t b; //0-255
} RgbColor;

//---------------------------------------------------------------------------

/*! \brief G³ówna klasa classKolor.

Umo¿liwia konwersjê miêdzy przestrzeniami kolorów
*/

class ColorUtils
{
public:
	static RgbColor HsvToRgb(HsvColor hsv); //!< Funkcja statyczna konwertuj¹ca HSV -> RGB, wszystie wartoœci //0-255
	static HsvColor RgbToHsv(RgbColor rgb); //!< Funkcja statyczna konwertuj¹ca RGB -> HSV, wszystie wartoœci //0-255
};
#endif
