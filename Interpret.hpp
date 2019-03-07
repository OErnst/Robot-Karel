#ifndef KAREL_INTERPRET_HPP
#define KAREL_INTERPRET_HPP

#include "Program.hpp"
#include "Robot.hpp"
#include "StaticTools.hpp"
#include "World.hpp"
#include <stack>
#include <string>

class Interpret {
private:
	Robot droid;
	Program program;
	void doWorldPart(std::istream& world);
	void doProgramPart(std::istream& program);
	int doProcedure();
	int breakX();
	void robotFails();
	int ifFunction(bool ifResult, int whichIf);
	int elseFunction();
	int identifyInstruction(int switchValue, const std::string& instruction);

public:
	/**
     * Parses input files and initializes the interpret.
     *
     * \param   world       stream with the world definition
     * \param   program     stream with the program source definition
     *
     * \note    Calls Complain::invalidSource, Complain::invalidWorld
     *          or Complain::undefinedReference on errors in the source.
     * \note    After successful initialization the interpret shall
     *          be in the \a running state with the \c MAIN as the
     *          active procedure.
     */
	Interpret(std::istream& world, std::istream& program);

	/**
     * Returns a constant reference to the Robot.
     * The robot changes as the program is being interpreted.
     */
	const Robot& robot() const;

	/**
     * Returns a constant reference to the World.
     * The world changes as the program is being interpreted.
     */
	const World& world() const;

	/**
     * Returns \c true if there exists a procedure named \p name.
     *
     * \param   name        name of the procedure to search for
     */
	bool hasProcedure(const std::string& name) const;

	/**
     * Returns \c true if the interpret is in the \a running state.
     */
	bool running() const;

	/**
     * Takes a single step in the program execution.
     * Does nothing if the interpret is in the \a stopped state.
     *
     * \returns \c true if the interpret remains in the \a running state.
     */
	bool step();

	/**
     * Executes all steps of the program until it ends.
     * Does nothing if the interpret is in the \a stopped state.
     *
     * \returns the number of steps the interpret took to finish the program
     */
	unsigned run();
};

#endif //KAREL_INTERPRET_HPP
