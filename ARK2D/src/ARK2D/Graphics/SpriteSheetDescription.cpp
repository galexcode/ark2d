/*
 * SpriteSheetDescription.cpp
 *
 *  Created on: 31 Jul 2010
 *      Author: Ashley
 */

#include "SpriteSheetDescription.h"
#include "../Util/StringUtil.h" 
#include "../Util/Cast.h"
#include "../Includes.h"
#include "../UI/ErrorDialog.h" 

namespace ARK {
	namespace Graphics {

		SpriteSheetDescription::SpriteSheetDescription(string path): ARK::Core::Resource(), m_path(path) {
			m_data = StringUtil::file_get_contents(path.c_str());
			load();
		}
		SpriteSheetDescription::SpriteSheetDescription(string path, void* rawData): ARK::Core::Resource(), m_path(path) {
			m_data = string((const char*) rawData);
			load();
		}

		void SpriteSheetDescription::load() { 
			std::cout << "Loading Spritesheet Description: " << m_path.c_str() << std::endl;
			if (m_data.length()==0) {
				ErrorDialog::createAndShow("Could not open spritesheet description.");
				exit(0);
			}

			if (StringUtil::getExtension(m_path).compare("spritesheetdescription") == 0) {
				string s(m_data);
				JSONNode* root = libJSON::Parse(s);

				JSONNode* sheet = root->GetNode("sheet");
				JSONNode* items = sheet->GetNode("items");
 
				for (unsigned int i = 0; i < items->NodeSize(); i++) {
					SpriteSheetDescriptionItem* it = new SpriteSheetDescriptionItem();

					JSONNode* item = items->NodeAt(i);
					it->setX( (unsigned int) item->GetNode("x")->NodeAsInt() );
					it->setY( (unsigned int) item->GetNode("y")->NodeAsInt() );
					it->setWidth( (unsigned int) item->GetNode("width")->NodeAsInt() );
					it->setHeight( (unsigned int) item->GetNode("height")->NodeAsInt() );
					it->setName( item->GetNode("name")->NodeAsString().c_str() );

					m_items[string(it->getName())] = (*it);
				}


				libJSON::Delete(root);
			} else { 	//if (StringUtil::getExtension(m_path).compare("txt") == 0) {
						//if (StringUtil::getExtension(m_path).compare("json") == 0 || {
				ErrorDialog::createAndShow("Use Resource loader. .txt and .json deprecated. Please use .spritesheetdescription");
				exit(0);
/*
				// get proper file path :|
				// TODO: Resource handling for spritesheets
				m_path = StringUtil::internalOSAppends(m_path); 
				std::cout << "txt spritesheet: " << m_path << std::endl;

				filebuf FileBuffer;
				FileBuffer.open(m_path.c_str(), ios::in);
				istream Stream(&FileBuffer);

				string Line;
				string Name, X, Y, Width, Height;


				while (!Stream.eof()) {
					std::getline(Stream, Line, '\n');

					if (Line.at(0) == '#') {
						//std::cout << "#" << std::endl;
						continue;
					}

					SpriteSheetDescriptionItem* item = new SpriteSheetDescriptionItem();
					string LineParam;
					stringstream LineStream;
					LineStream << Line;
					int ParamNumber = 0;
					while (ParamNumber < 5) {
						//std::cout << "Start ParamNumber: " << ParamNumber << std::endl;
						std::getline(LineStream, LineParam, ',');
						StringUtil::trim(LineParam, &" "[0]);
						if (ParamNumber == 0) {
							char* str = new char[LineParam.size() + 1];
							strcpy(str, LineParam.c_str());
							item->setName(str);
							//std::cout << "Set name" << std::endl;
						} else if (ParamNumber == 1) {

							char* str = new char[LineParam.size()];
							strcpy(str, LineParam.c_str());
							string string_object(str);
							unsigned int local_x = Cast::fromString<unsigned int>(string_object);
							item->setX(local_x);

							//std::cout << "Set x:" << local_x << std::endl;

						} else if (ParamNumber == 2) {

							char* str = new char[LineParam.size()];
							strcpy(str, LineParam.c_str());
							string string_object(str);
							unsigned int local_y = Cast::fromString<unsigned int>(string_object);
							item->setY(local_y);
							//std::cout << "Set y: " << std::endl;

						} else if (ParamNumber == 3) {
							char* str = new char[LineParam.size()];
							strcpy(str, LineParam.c_str());
							item->setWidth(Cast::fromString<unsigned int>(string(str)));
							//std::cout << "Set W" << std::endl;
						} else if (ParamNumber == 4) {
							char* str = new char[LineParam.size()];
							strcpy(str, LineParam.c_str());
							item->setHeight(Cast::fromString<unsigned int>(string(str)));
							//std::cout << "Set H" << std::endl;
						}
						//std::cout << "End ParamNumber: " << ParamNumber << std::endl;
						ParamNumber++;
					}
					//cout << item->getName() << " : " << item->getX() << " : " << item->getY() << " : " << item->getWidth() << " : " << item->getHeight() << endl;
					//std::cout << string(item->getName()) << std::endl;
					m_items[string(item->getName())] = (*item);

				}
				FileBuffer.close();
				*/
			}
			std::cout << "Loaded Spritesheet Description" << std::endl;
		}

		const SpriteSheetDescriptionItem& SpriteSheetDescription::getItemByName(const char* name) {
			return m_items[string(name)];
		}

		SpriteSheetDescription::~SpriteSheetDescription() {

		}
	}
}
