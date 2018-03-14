#include "gtest/gtest.h" // google test

#include "undo_redo_stack_interface.hpp" // interface to test
#include "undo_redo_stack_vector.hpp" // implementation to test

#include "command_interface.hpp"

#include <memory>

#include <stdexcept>


struct IncrementIntegerCommand : public CommandInterface
{
    static constexpr int kInitialValue = 42;
    int incrementValue = 1;
    int value = kInitialValue;
    bool executeWasCalled = false;
    bool undoWasCalled = false;

    IncrementIntegerCommand() = default;
    IncrementIntegerCommand(int f_incrementValue) : incrementValue(f_incrementValue) { }

    void execute() override
    {
        value += incrementValue;
        executeWasCalled = true;
    }
    void undo() override
    {
        value -= incrementValue;
        undoWasCalled = true;
    }

    int valueAfterSomeExecutes(int numberOfExecutes = 1)
    {
        if (numberOfExecutes < 0)
        {
            throw std::invalid_argument("numberOfExecutes > 0");
        }
        return kInitialValue + incrementValue * numberOfExecutes;
    }
};
constexpr int IncrementIntegerCommand::kInitialValue;

struct ThrowingCommand : public CommandInterface
{

    // CommandInterface interface
public:
    void execute() override { throw std::logic_error("this should get caught"); }
    void undo() override {}
};


class UndoRedoFactory : public ::testing::Test
{
protected:
    std::unique_ptr<UndoRedoStackInterface> undoRedoStackUnderTest;

    // handy for tests -- DRY
    IncrementIntegerCommand command;
    IncrementIntegerCommand commandA{1};
    IncrementIntegerCommand commandB{2};
    IncrementIntegerCommand commandC{3};

public:
    UndoRedoFactory()
    {
        undoRedoStackUnderTest.reset(new UndoRedoStackVector());
    }
};


TEST_F(UndoRedoFactory, commandIsExecuted)
{
    undoRedoStackUnderTest->execute(command);
    EXPECT_TRUE(command.executeWasCalled);
    EXPECT_EQ(command.value, command.valueAfterSomeExecutes());
    EXPECT_TRUE(undoRedoStackUnderTest->isAbleToUndo());
    EXPECT_FALSE(undoRedoStackUnderTest->isAbleToRedo());
}

TEST_F(UndoRedoFactory, contraryCommandIsExecutedOnUndo)
{
    undoRedoStackUnderTest->execute(command);
    EXPECT_TRUE(command.executeWasCalled);
    command.executeWasCalled = false; // reset to test after next redo

    EXPECT_TRUE(undoRedoStackUnderTest->isAbleToUndo());
    undoRedoStackUnderTest->undo();
    EXPECT_TRUE(command.undoWasCalled);
    EXPECT_EQ(command.value, command.kInitialValue);

    EXPECT_TRUE(undoRedoStackUnderTest->isAbleToRedo());
    undoRedoStackUnderTest->redo();
    EXPECT_TRUE(command.executeWasCalled);
    EXPECT_EQ(command.value, command.valueAfterSomeExecutes());
}

TEST_F(UndoRedoFactory, redoImpossibleAfterNewExecute)
{
    undoRedoStackUnderTest->execute(commandA);
    undoRedoStackUnderTest->execute(commandB);

    undoRedoStackUnderTest->undo();
    EXPECT_TRUE(undoRedoStackUnderTest->isAbleToUndo());
    EXPECT_TRUE(undoRedoStackUnderTest->isAbleToRedo());

    undoRedoStackUnderTest->execute(commandC);
    EXPECT_TRUE(undoRedoStackUnderTest->isAbleToUndo());
    EXPECT_FALSE(undoRedoStackUnderTest->isAbleToRedo());
}

TEST_F(UndoRedoFactory, throwIsForwarded)
{
    ThrowingCommand thrower;
    EXPECT_THROW(undoRedoStackUnderTest->execute(thrower), std::logic_error);
}

TEST_F(UndoRedoFactory, throwIsNotPutOnStack)
{
    ThrowingCommand thrower;
    EXPECT_THROW(undoRedoStackUnderTest->execute(thrower), std::logic_error);
    EXPECT_FALSE(undoRedoStackUnderTest->isAbleToUndo());
    EXPECT_FALSE(undoRedoStackUnderTest->isAbleToRedo());
}

TEST_F(UndoRedoFactory, throwOnEmptyUndo1)
{
    EXPECT_THROW(undoRedoStackUnderTest->undo(), std::out_of_range);
}

TEST_F(UndoRedoFactory, throwOnEmptyUndo2)
{
    undoRedoStackUnderTest->execute(command);
    undoRedoStackUnderTest->execute(command);
    undoRedoStackUnderTest->undo();
    undoRedoStackUnderTest->undo();
    EXPECT_THROW(undoRedoStackUnderTest->undo(), std::out_of_range);
}
