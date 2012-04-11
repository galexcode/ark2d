/*
 * BMFont.h
 *
 *  Created on: 13-Apr-2010
 *      Author: Ashley
 */

#ifndef BMFONT_H_
#define BMFONT_H_

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "../Includes.h"

#include "../Graphics/Image.h"
#include "../Graphics/Color.h"
#include "../Util/Log.h"
#include "../Util/StringUtil.h"

#include "Font.h"
#include "../ARK2D.h"

using namespace std;

namespace ARK {
	namespace Font {

		#ifndef DOXYGEN_SHOULD_SKIP_THIS
		/*!
		 * \ignore
		 * \brief CharDescriptor holds the information for a single character.
		 */

		struct CharDescriptor
		{
			//clean 16 bytes
			unsigned short x, y;
			unsigned short Width, Height;
			unsigned short XOffset, YOffset;
			unsigned short XAdvance;
			unsigned short Page;

			CharDescriptor() : x( 0 ), y( 0 ), Width( 0 ), Height( 0 ), XOffset( 0 ), YOffset( 0 ),
				XAdvance( 0 ), Page( 0 )
			{ }
		};
		// Charset holds the descriptions that apply to all characters, as well as the descriptors for every character.
		struct Charset
		{
			unsigned short LineHeight;
			unsigned short Base;
			unsigned short Width, Height;
			unsigned short Pages;
			CharDescriptor Chars[256];
		};
		struct FontVertex {
			float x, y, tu, tv;
		};

		#endif


		/*!
		 * \brief Bitmap / texture-mapped fonts generated by either Heiro or BMFont tools.
		 * These types of fonts are used by the framework by default.
		 *
		 * @author Ashley Gwinnell <info@ashleygwinnell.co.uk>
		 */
		class BMFont : public ARK::Font::Font {
			private:
				bool m_loaded;
				char* m_data;
				string m_FontFile;
				string m_ImageFile;
				Charset m_Charset;
				Image* m_Image;
				map<int, Image*> m_letterImages;

			public:
				BMFont();
				BMFont(unsigned int fntResource, unsigned int imgResource, unsigned int imgResourceType);
				BMFont(void* data, Image* i);
				BMFont(const string& f, const string& i);
				BMFont(const string& f, const string& i, const Color& mask);
				void drawString(const string& Str, int drawx, int drawy);
				unsigned int getStringWidth(const string& Str) const;
				unsigned int getStringHeight(const string& Str) const;
				unsigned int getLineHeight() const ;
				Image* getImage() const;
				virtual ~BMFont();
			private:
				bool Parse();


		};
	}
}

#endif /* FONT_H_ */
