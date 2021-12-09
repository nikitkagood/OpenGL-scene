#pragma once

#include "Shader.h"

class IModel
{
protected:
    IModel() = default;

    IModel(const IModel& copy) = delete;

    virtual void Draw(Shader& shader)
    {
        assert(false); //IModel::Draw is not meant to be called
    }
private:

};