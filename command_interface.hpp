#pragma once

class CommandInterface
{
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual ~CommandInterface(){}
};
