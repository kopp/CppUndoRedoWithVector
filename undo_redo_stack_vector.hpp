#pragma once

#include "undo_redo_stack_interface.hpp" // parent
#include <vector>

class UndoRedoStackVector : public UndoRedoStackInterface
{
protected:
    using ContainerType = std::vector<CommandInterface*>;
    using IndexType = ContainerType::size_type;

    ContainerType commandsThatWereExecutedSomeTime;
    IndexType indexNextCommandToRedo = 0;
    bool hasCommandsToRedoInVector = false;

public: // UndoRedoStackInterface interface
    void execute(CommandInterface& command) override;
    void undo() override;
    void redo() override;
    bool isAbleToUndo() override;
    bool isAbleToRedo() override;
};
