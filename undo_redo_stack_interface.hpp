#pragma once


#include "command_interface.hpp"

class UndoRedoStackInterface
{
public:
    virtual void execute(CommandInterface&) = 0;
    virtual void undo() = 0;
    virtual void redo() = 0;

    virtual bool isAbleToUndo() = 0;
    virtual bool isAbleToRedo() = 0;

    virtual ~UndoRedoStackInterface(){}
};

