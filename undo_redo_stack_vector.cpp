#include "undo_redo_stack_vector.hpp" // this

#include <stdexcept>

void UndoRedoStackVector::execute(CommandInterface& command)
{
    command.execute();
    if (hasCommandsToRedoInVector)
    {
        hasCommandsToRedoInVector = false;
        commandsThatWereExecutedSomeTime.erase(
                    commandsThatWereExecutedSomeTime.begin() + static_cast<ContainerType::difference_type>(indexNextCommandToRedo),
                    commandsThatWereExecutedSomeTime.end());
    }
    commandsThatWereExecutedSomeTime.push_back(&command);
}

void UndoRedoStackVector::undo()
{
    if (isAbleToUndo())
    {
        if (!hasCommandsToRedoInVector)
        {
            indexNextCommandToRedo = commandsThatWereExecutedSomeTime.size() - 1;
            hasCommandsToRedoInVector = true;
        }
        else
        {
            indexNextCommandToRedo -= 1;
        }
        CommandInterface* cmd = commandsThatWereExecutedSomeTime[indexNextCommandToRedo];
        cmd->undo();
    }
    else
    {
        throw std::out_of_range("Unable to undo since corresponding command has not been executed before.");
    }
}

void UndoRedoStackVector::redo()
{
    if (isAbleToRedo())
    {
        CommandInterface* cmd = commandsThatWereExecutedSomeTime[indexNextCommandToRedo];
        cmd->execute();
        indexNextCommandToRedo += 1;

        if (indexNextCommandToRedo == commandsThatWereExecutedSomeTime.size())
        {
            hasCommandsToRedoInVector = false;
        }
    }
    else
    {
        throw std::out_of_range("Unable to redo since corresponding command has not been undone before.");
    }
}

bool UndoRedoStackVector::isAbleToUndo()
{
    if (commandsThatWereExecutedSomeTime.empty())
    {
        return false;
    }
    else
    {
        if (hasCommandsToRedoInVector)
        {
            return indexNextCommandToRedo > 0;
        }
        else
        {
            return true;
        }
    }
}

bool UndoRedoStackVector::isAbleToRedo()
{
    return hasCommandsToRedoInVector;
}
