/*
 * LocalHighscores.cpp
 *
 *  Created on: 25 May 2011
 *      Author: Ashley
 */

#include "LocalHighscores.h"

#include "../ARK2D.h"
#include "../Core/GameContainer.h"

namespace ARK {
	namespace Util {

		LocalHighscores::LocalHighscores(string filename):
			ARK::Core::Resource(),
			m_filename(filename),
			m_data(NULL),
			m_items()
			{
			parse();
		}
		LocalHighscores::LocalHighscores(string filename, void* data):
			ARK::Core::Resource(),
			m_filename(filename),
			m_data(data),
			m_items() {
			parse();
		}

		vector<LocalHighscoreItem*> LocalHighscores::data() {
			return m_items;
		}

		string LocalHighscores::getFilename() {
			return m_filename;
		}
		void LocalHighscores::parse() {

			string s;
			#if defined(ARK2D_ANDROID)
				if (StringUtil::file_exists(m_filename.c_str())) {
					s = StringUtil::file_get_contents(m_filename.c_str());
				} else {
					s = string((char*) m_data);
				}
			#else

				if (m_data != NULL) {
					s = string((char*) m_data);
				} else {
					s = StringUtil::file_get_contents(m_filename.c_str());
				}
			#endif

			JSONNode* arr = libJSON::Parse(s);
			for(unsigned int i = 0; i < arr->NodeSize(); i++) {
				JSONNode* item = arr->NodeAt(i);

				LocalHighscoreItem* it = new LocalHighscoreItem();
				it->name = item->GetNode("name")->NodeAsString();
				it->score = item->GetNode("score")->NodeAsInt();
				m_items.push_back(it);
			}
			ARK2D::getLog()->i("loaded local highscores");

			this->sort();
		}
		/*void LocalHighscores::load() {
			char* data = StringUtil::file_get_contents(filename.c_str());
			if (data == NULL) {
				save();
				load();
			} else {
				string s = "";
				s.append(data);

			}
		}*/

		void LocalHighscores::sort() {
			std::sort(m_items.begin(), m_items.end(), LocalHighscores::mysortcomparator);
			std::cout << "sorted local highscores" << std::endl;
		}

		bool LocalHighscores::mysortcomparator(LocalHighscoreItem* one, LocalHighscoreItem* two) {
			return (one->score > two->score);
		}

		void LocalHighscores::addItem(string name, int score) {
			LocalHighscoreItem* item = new LocalHighscoreItem();
			item->name = name;
			item->score = score;
			m_items.push_back(item);
		}
		void LocalHighscores::save() {

			//! @todo: sort highscores.

			string s = "[";
				for(unsigned int i = 0; i < m_items.size(); i++) {
					LocalHighscoreItem* it = m_items.at(i);
					s.append("{");
						s.append("\"name\": \"");
						s.append(it->name);
						s.append("\",");
						s.append("\"score\": ");
						s.append(Cast::toString<int>(it->score));
					s.append("}");
					if (i != m_items.size()-1) {
						s.append(",");
					}
				}
			s.append("]");

			bool success = FileUtil::file_put_contents(m_filename, s);
			if (success) {
				std::cout << "saved local highscores" << std::endl;
			} else {
				std::cout << "could not save local highscores" << std::endl;
			}
		}

		LocalHighscores::~LocalHighscores() {

		}

	}
}
