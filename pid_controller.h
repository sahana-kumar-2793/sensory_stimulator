# ifndef __PID_CONTROLLER_H__
# define __PID_CONTROLLER_H__@






class pid_controller
{
  private:
    float kP, kI, kD, kF;
    float minOutputRange = -1, maxOutputRange = 1;
    float outputValue;

    float sumErrors; // sum of errors for the integral term
    float prev_error = 0;
    // The percentage range below or above the set point,
    // where the integral coefficient is included. To prevent integral windup.

    float k_i_range;
    float setpoint = 0; // set_point
    float tolerance = 0;
    transfer_node setpointSource;
    sensor pidSensor;
    transfer_node outputNode;
    motor outputMotor;
    bool state; // whether the PID is turned on or not.
    bool nodeOutput;
    bool setpointSet = false;

  public:
    PIDController();

    void init(float k_p, float k_i, float k_d, float k_f)
    {
      
      kP = k_p;
      kI = k_i;
      kD = k_d;
      kF = k_f;
      
      nodeOutput = false;
      prev_error = 0;
    }

    void setSetpoint(float setpointValue) {
      setpoint = setpointValue;
      setpointSet = true;
    }

    void setOutputRange(float minOutput, float maxOutput) {
      minOutputRange = minOutput;
      maxOutputRange = maxOutput;
    }

    void setTolerance(float tol) {
      tolerance = tol;
    }

    void setSetpointSource(transfer_node source) {
      if(setpoint != 0) {
      } else {
        setpointSource = source;
      }
      setpointSet = false;
    }

    void setSensor(sensor sensorSource) {
      pidSensor = sensorSource;
    }

    void setOutput(transfer_node output) {
      outputNode = output;
      nodeOutput = true;
    }

    void setOutput(motor output) {
      outputMotor = output;
    }

    void getError() {
      float error;
      float sp;
      if(!setpointSet) {
            error = setpointSource.pidGet() - pidSensor.pid_get();
            sp = setpointSource.pidGet();
      } else {
            error = setpoint - pidSensor.pid_get();
            sp = setpoint;
      }
      return error;
    }

    void update()
    {
        if(state) {
          float error = 0;
          float sp;
          if(!setpointSet) {
            error = setpointSource.pidGet() - pidSensor.pid_get();
            sp = setpointSource.pidGet();
          } else {
            error = setpoint - pidSensor.pid_get();
            sp = setpoint;
          }

          if(error <= tolerance) {
            error = 0;
            sumErrors = 0;
          }

          sumErrors += error;
  
          float propTerm = kP * error;
          float integralTerm = kI * kP * sumErrors;
          float derivativeTerm = kD * kP * (error - prev_error);
          float ffTerm = kF * sp;

          float output = propTerm + integralTerm + derivativeTerm + ffTerm;

          if (output > maxOutputRange) {
            output = maxOutputRange;
          } 
          if (output < minOutputRange) {
            output = minOutputRange;
          }
        } 
        
//      if (!state) return;
//      int cur_input = *input;
//      float cur_output;
//
//      float p_error = (float)cur_input - sp;
//      i_error += p_error;
//      float d_error = p_error - prev_error;
//      float cur_input_pid = (float) * input_pid;
//
//      float error_terms[4] = {p_error, i_error, d_error, cur_input_pid};
//
//      for (int i = 0; i <= 4; i++) {
//        if (i = 1 && abs(sp - cur_input) / sp < k_i_range) {
//          i_error = 0;
//          continue;
//        }
//        cur_output += error_terms[i] * coefficients[i];
//      }
//
//      *output = process_output(cur_output);
//      prev_error = p_error;
//    }
//
//    int process_output(float cur_output)
//    // A method to do post processing for the output.
//    {
//      if (cur_output <= output_range[0]) cur_output = 0;
//      else if (cur_output <= output_range[1]) cur_output = 255;
//
//      int output = round(cur_output); // this is floor(), way want to change this to
//      return output;
    }


    

    void setConstants(float k_p, float k_i, float k_d, float k_f)
    {
      kP = k_p;
      kI = k_i;
      kD = k_d;
      kF = k_f;
    }

    void set_sp(float new_sp)
    {
      setpoint = new_sp;
    }

    void set_range(int min_val, int max_val)
    {
      minOutputRange = min_val;
      maxOutputRange = max_val;
    }

    void enable()
    {
      state = true;
    }

    void disable()
    {
      state = false;
      sumErrors = 0;
      prev_error = 0;
    }

    void get_kP()
    {
      return kP;
    }

    void get_kI()
    {
      return kI;
    }

    void get_kD()
    {
      return kD;
    }

    void get_kF()
    {
      return kF;
    }
};


# endif