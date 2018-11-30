

function conveyor_connect()
     "Opens the serial port to communicate with the robot arm and sets some \
defaults"
{
  conveyor_init();
};

function conveyor_disconnect()
     "Closes the connection"
{
  conveyor_quit();
};

function conveyor_move()
    "Advances the conveyor"
{
    move();
};

function conveyor_res()
    "Gets the resistance"
{
    res();
};

function send_command(command)
     "Send a <set curtag red>non motion<set curtag reg> command string to \
the arm. Non-motion commands do not wait for the motion to complete. \
They simply write the command and swallow all output from the serial port. \
Motion commands should use <set curtag See>send motioncommand<set curtag reg>."
{
  local res, comlen;

  res = write_cmnd(command);
  res;
};

function send_motioncommand(command)
     "Send a <set curtag red>motion<set curtag reg> command string to the \
arm. This executes the requested command, and then executes a \
joint 1,0 command to move joint 1 by 0 degrees. We then wait until \
this second command returns. Non motion commands should use \
<set curtag See>send_command<set curtag reg>."
{
  local res;
  res = send_command(command);
  sleep(10);
  res;
};

