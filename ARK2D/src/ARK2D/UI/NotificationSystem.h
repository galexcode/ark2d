/*
 * NotificationSystem.h
 *
 *  Created on: 02 Sep 2012
 *      Author: Ashley
 */

#ifndef ARK_CORE_NOTIFICATIONSYS_H_
#define ARK_CORE_NOTIFICATIONSYS_H_ 

#include "../ARK2D.h"
#include "../Namespaces.h"
#include "../Includes.h"
#include "../Util/Containers/Vector.h"


namespace ARK {
	namespace UI {
		class Notification;

		class NotificationSystem {
			
			private:
				ARK::Util::Containers::Vector<ARK::UI::Notification* > m_notifications;
				float m_defaultDuration;

			public:
				NotificationSystem();
				void add(ARK::UI::Notification* n);
				void normal(string title, string body);
				void info(string body);
				void success(string body);
				void error(string body);
				void update();
				void render();
				virtual ~NotificationSystem();
		};
	}
}

#endif /* ARK_CORE_NOTIFICATIONSYS_H_ */
