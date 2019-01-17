//---------------------------------------------------------------------------
#ifndef ColorUtilsH
#define ColorUtilsH

#include <stdint.h>
//---------------------------------------------------------------------------
/*! \file classKolor.h Plik zawieraj�cy klas� 'classKolor'.
*/
/*! \brief Klasa opisuj�ca kolor wg modelu SVH.
*/
typedef struct HsvColor
{
	uint8_t h; //0-255
	uint8_t s; //0-255
	uint8_t v; //0-255
} HsvColor;
//---------------------------------------------------------------------------
/*! \brief Klasa opisuj�ca kolor wg modelu RGB.
*/
typedef struct RgbColor
{
	uint8_t r; //0-255
	uint8_t g; //0-255
	uint8_t b; //0-255
} RgbColor;

//---------------------------------------------------------------------------

/*! \brief G��wna klasa classKolor.

Umo�liwia konwersj� mi�dzy przestrzeniami kolor�w
*/

class ColorUtils
{
public:
	static RgbColor HsvToRgb(HsvColor hsv); //!< Funkcja statyczna konwertuj�ca HSV -> RGB, wszystie warto�ci //0-255
	static HsvColor RgbToHsv(RgbColor rgb); //!< Funkcja statyczna konwertuj�ca RGB -> HSV, wszystie warto�ci //0-255
};
#endif
