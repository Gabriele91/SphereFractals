#ifndef INPUT_H
#define INPUT_H

#include <Config.h>
#include <Debug.h>
#include <Math2D.h>

namespace Sphere {

		namespace Key{
			extern const char *stringKeyboard[];
			extern const char *stringMouse[];
			extern const char *stringFinger[];
			enum Keyboard{
				KEY_NULL=0,
				BACKSPACE=1,
				TAB,
				CLEAR,
				RETURN,
				PAUSE,
				ESCAPE,
				SPACE,
				QUOTE,
				APOSTROPHE,
				COMMA,
				MINUS,
				PERIOD,
				SLASH,
				N0,
				N1,
				N2,
				N3,
				N4,
				N5,
				N6,
				N7,
				N8,
				N9,
				SEMICOLON,
				EQUALS,
				LEFTBRACKET,
				BACKSLASH,
				LESS,
				RIGHTBRACKET,
				BACKQUOTE,
				A,
				B,
				C,
				D,
				E,
				F,
				G,
				H,
				I,
				J,
				K,
				L,
				M,
				N,
				O,
				P,
				Q,
				R,
				S,
				T,
				U,
				V,
				W,
				X,
				Y,
				Z,
				KDELETE,
				KP0,
				KP1,
				KP2,
				KP3,
				KP4,
				KP5,
				KP6,
				KP7,
				KP8,
				KP9,
				KP_PERIOD,
				KP_DIVIDE,
				KP_MULTIPLY,
				KP_MINUS,
				KP_PLUS,
				UP,
				DOWN,
				RIGHT,
				LEFT,
				INSERT,
				HOME,
				END,
				PAGEUP,
				PAGEDOWN,
				F1,
				F2,
				F3,
				F4,
				F5,
				F6,
				F7,
				F8,
				F9,
				F10,
				F11,
				F12,
				F13,
				F14,
				F15,
				NUMLOCK,
				CAPSLOCK,
				SCROLLOCK,
				RSHIFT,
				LSHIFT,
				RCTRL,
				LCTRL,
				RALT,
				LALT,
				RMETA,
				LMETA,
				HELP,
				PRINT,
				BREAK,
				MENU,
				POWER,
				KEYBOARDMAX = 323
			};
			enum Mouse{
				BUTTON_LEFT=1,
				BUTTON_MIDDLE,
				BUTTON_RIGHT,
				BUTTON4,
				BUTTON5,
				BUTTON6,
				MOUSEMAX
			};
			enum Finger{
				FINGER1=1,
				FINGER2,
				FINGER3,
				FINGER4,
				FINGER5,
				FINGER6,
				FINGER7,
				FINGER8,
				FINGER9,
				FINGER10,
				FINGERMAX
			};
		};

		/**
		* @class Input
		*
		* @brief abstract class input management
		*
		* @author Gabriele Di Bari
		*
		*/
		class Input{

		public:

			class KeyboardHandler{
				public:
					//keyboard
					virtual void onKeyPress(Key::Keyboard key) {}
					virtual void onKeyRelease(Key::Keyboard key) {}
            };	
			class FingersHandler{
				public:
					//finger touch
					virtual void onFingerMove(Vec2 touchPosition,Key::Finger FingerID )  {}
					virtual void onFingerPress(Vec2 touchPosition,Key::Finger FingerID ) {}
					virtual void onFingerRelease(Vec2 touchPosition,Key::Finger FingerID )   {}
            };				
			class AccelerometerHandler{
				public:
					//accelerometer
					virtual void onAcceleration( Vec3 accel, float roll )	{}
			};
			class MouseHandler{		
				public:
					//mouse
					virtual void onMouseMove(Vec2 mousePosition) {}
					virtual void onMousePress(Vec2 mousePosition, Key::Mouse button) {}
					virtual void onMouseRelease(Vec2 mousePosition, Key::Mouse button) {}
					virtual void onMouseScroll(short scrollDelta) {}
			};

			/**
			* Destructor
			*/
			virtual ~Input(){};
			/**
			* get mouse position
			* @return mouse position
			*/
			virtual Vec2 getMouse() const=0;
			/**
			* get finger position
			* @param finger number
			* @return finger position
			*/
			virtual Vec2 getFinger(Key::Finger id) const=0;
			/**
			* get accelerometer values
			* @return (x,y,z) axes accelerometer values
			*/
			virtual Vec3 getAccelerometer() const=0;
			/**
			* update window event,
			* call this method in a loop
			*/
			virtual void update()=0;
			/**
			* Return true if window has focus
			* @return focus status
			*/
			virtual bool getFocus() const=0;
			/**
			* Return true if keyboard button is down
			* @param key's id
			* @return status
			*/
			virtual bool getKeyDown(Key::Keyboard id) const=0;
			/**
			*  Return true if go up keyboard button
			* @param key's id
			* @return status
			*/
			virtual bool getKeyUp(Key::Keyboard id) const=0;
			/**
			*  Return true if hit keyboard button
			* @param key's id
			* @return status
			*/
			virtual bool getKeyHit(Key::Keyboard id) const=0;
			/**
			* Return true if mouse button is down
			* @param mouse button
			* @return status
			*/
			virtual bool getMouseDown(Key::Mouse id) const=0;
			/**
			* Return true if mouse button is up
			* @param mouse button
			* @return status
			*/
			virtual bool getMouseUp(Key::Mouse id) const=0;
			/**
			* Return true if mouse button is hit
			* @param mouse button
			* @return status
			*/
			virtual bool getMouseHit(Key::Mouse id) const=0;
			/**
			* Return scroll wheel delta
			* @return scroll delta
			*/
			virtual float getScroll() const=0;
			/**
			* Return true if finger is down
			* @param finger number
			* @return status
			*/
			virtual bool getFingerDown(Key::Finger id) const=0;
			/**
			* Return true if finger is up
			* @param finger number
			* @return status
			*/
			virtual bool getFingerUp(Key::Finger id) const=0;
			/**
			* Return true if finger hit the screen
			* @param  finger number
			* @return status
			*/
			virtual bool getFingerHit(Key::Finger id) const=0;
			/**
			* Return true if window minimized
			* @return window status
			*/
			virtual bool getMinimized() const=0;
			/**
			* Return true if window maximized
			* @return window status
			*/
			virtual bool getMaximized() const=0;
			/**
			* Return true if window is closed
			* @return window status
			*/
			virtual bool getClose() const=0;
			/**
			* Return true if window is resized
			* @return window status
			*/
			virtual bool getResize() const=0;
			/**
			* window size (when it is resized)
			* @return window size
			*/
			virtual Vec2 getResizeValues() const=0;
			/**
			* Adds an input keyboard handler to the callback list
			*/
			virtual void addHandler(KeyboardHandler *handler){
				DEBUG_ASSERT(handler);
				vkeyboardh.push_back(handler);
			}
			/**
			* Adds an input fingers handler to the callback list
			*/
			virtual void addHandler(FingersHandler *handler){
				DEBUG_ASSERT(handler);
				vfingersh.push_back(handler);
			}
			/**
			* Adds an input accelerometer handler to the callback list
			*/
			virtual void addHandler(AccelerometerHandler *handler){
				DEBUG_ASSERT(handler);
				vaccelerometerh.push_back(handler);
			}
			/**
			* Adds an input mouse handler to the callback list
			*/
			virtual void addHandler(MouseHandler *handler){
				DEBUG_ASSERT(handler);
				vmouseh.push_back(handler);
			}
			/**
			* Removes an input keyboard handler from the callback list
			*/
			virtual void removeHandler(KeyboardHandler *handler){	
				DEBUG_ASSERT(handler);
				for(auto it=vkeyboardh.begin();it!=vkeyboardh.end();++it) {
					if(*it==handler) { 
						vkeyboardh.erase(it); 
						break; 
					}
				}
			}
			/**
			* Removes an input fingers handler from the callback list
			*/
			virtual void removeHandler(FingersHandler *handler){	
				DEBUG_ASSERT(handler);
				for(auto it=vfingersh.begin();it!=vfingersh.end();++it) {
					if(*it==handler) { 
						vfingersh.erase(it); 
						break; 
					}
				}
			}
			/**
			* Removes an input accelerometer handler from the callback list
			*/
			virtual void removeHandler(AccelerometerHandler *handler){	
				DEBUG_ASSERT(handler);
				for(auto it=vaccelerometerh.begin();it!=vaccelerometerh.end();++it) {
					if(*it==handler) { 
						vaccelerometerh.erase(it); 
						break; 
					}
				}
			}
			/**
			* Removes an input mouse handler from the callback list
			*/
			virtual void removeHandler(MouseHandler *handler){	
				DEBUG_ASSERT(handler);
				for(auto it=vmouseh.begin();it!=vmouseh.end();++it) {
					if(*it==handler) { 
						vmouseh.erase(it); 
						break; 
					}
				}
			}

			protected:
				
				std::vector<KeyboardHandler*> vkeyboardh;
				std::vector<FingersHandler*> vfingersh;
				std::vector<AccelerometerHandler*> vaccelerometerh;
				std::vector<MouseHandler*> vmouseh;
				/*
				//window
				struct EventWindow{
					bool focus,
						 minimized,
						 maximized,
						 resize,
						 close;
					Vec2 windowResize;
				}ewindow;
				//mouse			
				struct EventMouse{					
					int nPress;
					short scroll;
					Vec2 pos;
					bool status[Key::KEYBOARDMAX];
					Key::Keyboard hit[10];
				}emouse;
				//fingers
				struct EventFinges{
					int nPress;
					Vec2 pos[Key::FINGERMAX];
					bool status[Key::FINGERMAX];//[2];
					Key::Finger hit[Key::FINGERMAX];
				}efingers;
				//keyboard
				struct EventKeyboard{
					int nPress;
					Key::Keyboard hit[10];
					bool status[Key::KEYBOARDMAX];//[2];
				}ekeyboard;
				*/

	};

};

#endif