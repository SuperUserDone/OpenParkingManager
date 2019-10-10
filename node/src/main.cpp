/*
OpenParkingManager - An open source parking manager and parking finder.
    Copyright (C) 2019 Louis van der Walt

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or 
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <iostream>

#include <cppgpio.hpp>

typedef struct
{
    bool a0;
    bool a1;
    bool b0;
    bool b1;
} stepper_motor_inputs_t;

stepper_motor_inputs_t halfstep_seq[8] = {
    {1,0,0,0},
    {1,1,0,0},
    {0,1,0,0},
    {0,1,1,0},
    {0,0,1,0},
    {0,0,1,1},
    {0,0,0,1},
    {1,0,0,1}
};

bool reverse;
uint16_t step_position = 32760;

GPIO::DigitalOut motor_a0(6);
GPIO::DigitalOut motor_a1(13);
GPIO::DigitalOut motor_b0(26);
GPIO::DigitalOut motor_b1(19);

GPIO::DigitalIn Limit0(20, GPIO::GPIO_PULL::UP);
GPIO::DigitalIn Limit1(16, GPIO::GPIO_PULL::UP);

void set_outputs(stepper_motor_inputs_t in)
{
    if(in.a0)
    {
        motor_a0.on();
    }
    else
    {
        motor_a0.off();
    }
    
    if(in.a1)
    {
        motor_a1.on();
    }
    else
    {
        motor_a1.off();
    }
    
    if(in.b0)
    {
        motor_b0.on();
    }
    else
    {
        motor_b0.off();
    }

    if(in.b1)
    {
        motor_b1.on();
    }
    else
    {
        motor_b1.off();
    }
}

void check_limits()
{
    if(Limit0.triggered())
        std::cout << "Limit 0" << std::endl;
    if(Limit0.get_state())
    {
        reverse = !reverse;
    }
    if(Limit1.triggered())
         std::cout << "Limit 1" << std::endl;
    if(Limit1.get_state())
    {
        reverse = !reverse;
    }
}

void move(int steps)
{
    for(int i = 0; i < steps; i++)
    {
        set_outputs(halfstep_seq[step_position % 8]);
        if(!reverse)
            step_position++;
        else
            step_position--;
        check_limits();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main()
{
    move(16);
    return 0;
}