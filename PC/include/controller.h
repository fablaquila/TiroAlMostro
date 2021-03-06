#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <QObject>
#include <QQuickView>
#include <QSettings>
#include <QList>
#include <QVariant>
#include <QTimer>
#include "gameController.h"
#include "serialCommunication.h"
#include "joystickPointer.h"
#include "whacAMaker.h"

/**
 * \brief The main controller of the game
 *
 * This class takes care of controlling the game. It connects to signals from
 * QML items, starts the thread to communicate with Arduino and reads/store
 * configuration parameters and scores
 */
class Controller : public QObject
{
	Q_OBJECT

	/**
	 * \brief The current application status
	 */
	enum Status {
		Menu,
		JoystickCalibration,
		Game
	};

public:
	/**
	 * \brief Constructor
	 *
	 * \param view the object displaying the QML scene and starting the QML
	 *             engine
	 * \param parent the parent object
	 */
	Controller(QQuickView& view, QObject* parent = NULL);

	/**
	 * \brief Destructor
	 */
	virtual ~Controller();

	/**
	 * \brief Returns a const reference to the QML view
	 *
	 * \return a const reference to the QML view
	 */
	const QQuickView& getView() const
	{
		return m_view;
	}

	/**
	 * \brief Returns a reference to the QML view
	 *
	 * \return a reference to the QML view
	 */
	QQuickView& view()
	{
		return m_view;
	}

	/**
	 * \brief If score is going to be a highscore for the given level,
	 *        returns true and uses it as the score of the next player
	 *
	 * \param modality the game modality
	 * \param level the difficulty level
	 * \param score the score
	 * \return true if score is in the highscores
	 */
	bool newHighScore(WhacAMaker::GameType modality, WhacAMaker::DifficultyLevel level, double score);

	/**
	 * \brief The function called when a new command is received
	 *
	 * Here we call methods of the SerialCommunication object to obtain the
	 * command that has been received
	 */
	void commandReceived();

private slots:
	/**
	 * \brief This is the slot called when configuration parameters are
	 *        saved
	 *
	 * \param obj the object with configuration parameters
	 */
	void saveConfigurationParameters();

	/**
	 * \brief This is the slot called when a player has entered its name
	 *
	 * \param name the name of the player
	 */
	void savePlayerName(const QString& name);

	/**
	 * \brief The slot called when joystick calibration starts
	 */
	void joystickCalibrationStarted();

	/**
	 * \brief The slot called when joystick calibration ends
	 */
	void joystickCalibrationEnded();

	/**
	 * \brief The slot called with the new position of the joystick pointer
	 *        on the screen
	 *
	 * This function emulates mouse clicks if buttons are pressed
	 * \param x the x position of the pointer on the main view frame of
	 *          reference
	 * \param y the y position of the pointer on the main view frame of
	 *          reference
	 * \param button1Pressed whether the first button is pressed or not
	 * \param button2Pressed whether the second button is pressed or not
	 */
	void pointerPosition(qreal x, qreal y, bool button1Pressed, bool button2Pressed);

	/**
	 * \brief The slot called when the game starts
	 */
	void gameStarted();

	/**
	 * \brief The slot called when the game ends
	 */
	void gameFinished();

	/**
	 * \brief Resizes the movement area of the joystick
	 */
	void resizeJoystickMovementArea();

	/**
	 * \brief Deactivates the servos
	 *
	 * This is called by a timer to be sure all servos stopped moving
	 */
	void disableServos();

private:
	/**
	 * \brief Restores parameters in the configuration parameters QML object
	 */
	void restoreParameters();

	/**
	 * \brief Restores the highscores in the panel for the given level
	 *
	 * \param modality the game modality whose highscores to restore
	 * \param level the difficulty level whose highscores to restore
	 */
	void restoreHighScores(WhacAMaker::GameType modality, WhacAMaker::DifficultyLevel level);

	/**
	 * \brief Copies a value from settings to an QML item
	 *
	 * \param item the QML item whose property we write
	 * \param propName the name of the property to write. The name of the
	 *                 property is "\<propName\>Value" and the name of the
	 *                 setting is "configuration/\<propName\>"
	 */
	void copyPropertyToItem(QObject* item, QString propName);

	/**
	 * \brief Copies a value from QML item to settings if valid, restores
	 *        a valid value in the QML item otherwise
	 *
	 * \param item the QML item whose property we read
	 * \param propName the name of the property to read. The name of the
	 *                 property is "\<propName\>Value" and the name of the
	 *                 setting is "configuration/\<propName\>"
	 * \return true if the user value was valid, false otherwise
	 */
	bool copyPropertyToSettings(QObject* item, QString propName);

	/**
	 * \brief Returns the highscores and players for the given level loading
	 *        them from m_setting
	 *
	 * \param modality the game modality whose highscores and players to get
	 * \param level the level whose highscores and players to get
	 * \param highscores the vector to fill with highscores
	 * \param players the vector to fill with players
	 * \param modalityName the string representation of game modality
	 * \param levelName the string representation of level
	 */
	void getHighScoresFromSettings(WhacAMaker::GameType modality, WhacAMaker::DifficultyLevel level, QList<QVariant>& highscores, QList<QVariant>& players, QString& modalityName, QString& levelName);

	/**
	 * \brief Sets the serial port in the serial communication object
	 *        reading it from settings
	 */
	void setSerialPort();

	/**
	 * \brief Sets the volume of sound effects
	 */
	void setAudioVolume();

	/**
	 * \brief Sets the game area size and position
	 *
	 * Values are only set if they are present in m_settings
	 */
	void setGameAreaSizeAndPosition();

	/**
	 * \brief The current status
	 */
	Status m_status;

	/**
	 * \brief The object with settings for the application
	 */
	QSettings m_settings;

	/**
	 * \brief The object displaying the QML scene and starting the QML
	 *        engine
	 */
	QQuickView& m_view;

	/**
	 * \brief The object taking care of the communication with Arduino
	 */
	SerialCommunication m_serialCom;

	/**
	 * \brief The object managing the joystick pointer
	 */
	JoystickPointer m_joystickPointer;

	/**
	 * \brief The object controlling the game
	 */
	GameController m_gameController;

	/**
	 * \brief The modality of the next highscore
	 */
	WhacAMaker::GameType m_nextScoreModality;

	/**
	 * \brief The level of the next highscore
	 */
	WhacAMaker::DifficultyLevel m_nextScoreLevel;

	/**
	 * \brief The score of the next highscore
	 */
	double m_nextScore;

	/**
	 * \brief The previous x position of the joystick pointer
	 */
	qreal m_joystickPrevX;

	/**
	 * \brief The previous y position of the joystick pointer
	 */
	qreal m_joystickPrevY;

	/**
	 * \brief The previous status of button 1 (whether pressed or not)
	 */
	bool m_button1PrevStatus;

	/**
	 * \brief The previous status of button 2 (whether pressed or not)
	 */
	bool m_button2PrevStatus;

	/**
	 * \brief The timer with the delay before disabling servos
	 */
	QTimer m_servoDisablingTimer;
};

#endif
