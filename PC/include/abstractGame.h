#ifndef __ABSTRACT_GAME_H__
#define __ABSTRACT_GAME_H__

#include <QObject>
#include "whacAMaker.h"

class GameController;

/**
 * \brief The abstract class implmenting the game logic
 *
 * This is the abstract class for the game logic. Subclass it to implement a new
 * game logic. For the moment you also have to modify GameController to
 * instantiate the new class when needed
 */
class AbstractGame : public QObject
{
	Q_OBJECT

public:
	/**
	 * \brief Constructor
	 *
	 * \param controller the game controller object
	 */
	AbstractGame(GameController* controller);

	/**
	 * \brief Destructor
	 */
	virtual ~AbstractGame();

	/**
	 * \brief Starts the game
	 *
	 * \param difficulty the difficulty level
	 */
	virtual void startGame(WhacAMaker::DifficultyLevel difficulty) = 0;

	/**
	 * \brief Stops the game
	 *
	 * This is called when the game should stop for external reasons
	 */
	virtual void stopGame() = 0;

	/**
	 * \brief The current joystick pointer status
	 *
	 * \param moleID the id of the mole under the pointer
	 * \param button1Pressed the status of the first button
	 * \param button2Pressed the status of the second button
	 */
	virtual void pointerStatus(int moleID, bool button1Pressed, bool button2Pressed) = 0;

	/**
	 * \brief The player score
	 *
	 * \return the player score
	 */
	virtual qreal score() const = 0;

protected:
	/**
	 * \brief Returns true if the game is running
	 */
	bool gameRunning() const;

	/**
	 * \brief The game controller object
	 */
	GameController* const m_controller;

private:
	/**
	 * \brief If true the game has started
	 */
	bool m_gameRunning;

	/**
	 * \brief GameController is friend to modify the m_gameRunning flag
	 */
	friend class GameController;
};

#endif
