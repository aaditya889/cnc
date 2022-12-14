#include <Arduino.h>
#include <math.h>



double *read_coordinates(String gcode_line)
{
    int x_st_idx = -1, x_en_idx = -1, y_st_idx = -1, y_en_idx = -1, r_st_idx = -1, r_en_idx = -1;
    unsigned int curr_char = 0;
    double *parameters = (double *) malloc(sizeof(double) * 3);

    while (gcode_line.charAt(curr_char) != 'X') curr_char++;
    curr_char++;
    x_st_idx = curr_char;
    while (gcode_line.charAt(curr_char) != ' ') curr_char++;
    x_en_idx = curr_char;
    while (gcode_line.charAt(curr_char) != 'Y') curr_char++;
    curr_char++;
    y_st_idx = curr_char;
    while ((curr_char != (gcode_line.length() + 1)) && (gcode_line.charAt(curr_char) != '\n') && (gcode_line.charAt(curr_char) != 'R')) curr_char++;
    y_en_idx = curr_char;

    if ((curr_char != (gcode_line.length() + 1)) && (gcode_line.charAt(curr_char) == 'R'))
    {
        curr_char++;
        r_st_idx = curr_char;
        while ((curr_char != (gcode_line.length() + 1)) && (gcode_line.charAt(curr_char) != '\n')) curr_char++;
        r_en_idx = curr_char;
    }

    parameters[0] = (double) gcode_line.substring(x_st_idx, x_en_idx).toDouble();
    parameters[1] = (double) gcode_line.substring(y_st_idx, y_en_idx).toDouble();
    if (r_st_idx != -1 && r_en_idx != -1) parameters[2] = gcode_line.substring(r_st_idx, r_en_idx).toDouble();
    else parameters[2] = INT_MIN;

    // Serial.printf("r_st_idx: %d, r_en_idx: %d\n", r_st_idx, r_en_idx);
    // Serial.printf("String parameters:: (%s, %s)", gcode_line.substring(x_st_idx, x_en_idx).c_str(), gcode_line.substring(y_st_idx, y_en_idx).c_str());

    // if (r_en_idx != -1 && r_st_idx != -1) Serial.printf(", radius: %s\n", gcode_line.substring(r_st_idx, r_en_idx).c_str());
    // else Serial.printf("\n");

    return parameters;
    
}


void interpret_gcode(String gcode_line)
{
    double *parameters = read_coordinates(gcode_line);
    
    Serial.printf("Got parameters (original frame): (%4f, %4f)", parameters[0], parameters[1]);
    if (parameters[2] != INT_MIN) Serial.printf(", and radius: %4f\n", parameters[2]);
    else Serial.printf("\n");

    BLA::Matrix<2> transformed_coordinates = ROTATION_MATRIX * (BLA::Matrix<2>) {parameters[0], parameters[1]};
    parameters[0] = transformed_coordinates(0);
    parameters[1] = transformed_coordinates(1);

    if (DEBUG_SWITCHES[IS_INCHES_IDX]) 
    {
        if (DEBUG_SWITCHES[VERBOSE_LOGGING_IDX]) Serial.printf("Converting inches to mm...\n");
        parameters[0] *= INCH_TO_MM_FACTOR;
        parameters[1] *= INCH_TO_MM_FACTOR;
        if (parameters[2] != INT_MIN) parameters[2] *= INCH_TO_MM_FACTOR; 
    }

    Serial.printf("Got parameters (transformed frame): (%4f, %4f)\n", parameters[0], parameters[1]);


    if (gcode_line.startsWith("G00 ") || gcode_line.startsWith("G0 "))
    {   
        // Place the tool without engaging it!
        disengage_z_axis();
        // Take care of the vibrations
        delay(100);
        ;
        draw_line_mm(INT_MIN, INT_MIN, parameters[0], parameters[1]);
    }
    else if (gcode_line.startsWith("G01 ") || gcode_line.startsWith("G1 "))
    {
        engage_z_axis();
        ;
        delay(100);
        // Take care of the vibrations
        
        draw_line_mm(INT_MIN, INT_MIN, parameters[0], parameters[1]);
    }
    else if (gcode_line.startsWith("G02 ") || gcode_line.startsWith("G2 "))
    {
        engage_z_axis();
        ;
        delay(100);
        // Take care of the vibrations
        
        draw_arc_radius_mm(INT_MIN, INT_MIN, parameters[0], parameters[1], parameters[2]);
    }
    free(parameters);
}


boolean is_gcode_line_valid(String gcode_line_received)
{
    if (
    !(
        // Whitelist
        gcode_line_received.startsWith("G1") 
    ||  gcode_line_received.startsWith("G01") 
    ||  gcode_line_received.startsWith("G2") 
    ||  gcode_line_received.startsWith("G0") 
    ||  gcode_line_received.startsWith("G02")
    ||  gcode_line_received.startsWith("G00")
    ) ||
    (
        // Blacklist
        (gcode_line_received.indexOf("Z") != -1)
    ||  false
    )
    )
        return false;

    return true;
}

boolean add_gcode_line_to_queue(String gcode_line)
{   
    Serial.printf("Adding GCode line: %s\n", gcode_line.c_str());
    // free()

    if (((END_GCODE_LINE_IDX + 1) % MAX_GCODE_LINES) == START_GCODE_LINE_IDX)
    {
        Serial.printf("GCode Queue full!\n");
        // Handling for full queue
        return false;
    }
    
    if (!is_gcode_line_valid(gcode_line)) 
    {
        Serial.printf("No support for G-Code line: %s, skipping...\n", gcode_line.c_str());
        return false;
    }

    // GCODE_LINES[END_GCODE_LINE_IDX].clear();
    // GCODE_LINES[END_GCODE_LINE_IDX].concat(gcode_line);
    strcpy(GCODE_LINES[END_GCODE_LINE_IDX], gcode_line.c_str());
    // Serial.printf("Copied the string to char * array: %s\n", GCODE_LINES[END_GCODE_LINE_IDX]);
    // GCODE_LINES[END_GCODE_LINE_IDX] = gcode_line;
    END_GCODE_LINE_IDX = (END_GCODE_LINE_IDX + 1) % MAX_GCODE_LINES; 
    Serial.printf("End_idx: %d, Start_idx: %d\n", END_GCODE_LINE_IDX, START_GCODE_LINE_IDX);
    // free(&gcode_line);
    return true;
}


void read_and_execute_gcode_lines()
{
    int current_gcode_line_end_idx = END_GCODE_LINE_IDX;
    // Serial.printf("Remaining Heap Mem: %u\n", ESP.getFreeHeap());

    while (START_GCODE_LINE_IDX != current_gcode_line_end_idx)
    {
        interpret_gcode(GCODE_LINES[START_GCODE_LINE_IDX]);
        START_GCODE_LINE_IDX = (START_GCODE_LINE_IDX + 1) % MAX_GCODE_LINES;
    }
}