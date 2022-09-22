#include <Arduino.h>
#include <math.h>
#include "line_functions.h"
#include "arc_functions.h"


double *read_coordinates(String gcode_line)
{
    unsigned int curr_char = 0, x_st_idx = -1, x_en_idx = -1, y_st_idx = -1, y_en_idx = -1, r_st_idx = -1, r_en_idx = -1;
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

    parameters[0] = gcode_line.substring(x_st_idx, x_en_idx).toDouble();
    parameters[1] = gcode_line.substring(y_st_idx, y_en_idx).toDouble();
    if (r_st_idx && r_en_idx) parameters[2] = gcode_line.substring(r_st_idx, r_en_idx).toDouble();
    else parameters[2] = INT_MIN;

    Serial.print("String parameters (X, Y, (R)):: ");
    Serial.print(gcode_line.substring(x_st_idx, x_en_idx));
    Serial.print(" ");
    Serial.print(gcode_line.substring(y_st_idx, y_en_idx));
    if (r_en_idx && r_st_idx)
    {
        Serial.print(" ");
        Serial.println(gcode_line.substring(r_st_idx, r_en_idx));
    }
    else Serial.println("");

    return parameters;
    
}


void interpret_gcode(String gcode_line)
{
    double *parameters = read_coordinates(gcode_line);

    Serial.print("Got parameters (X, Y, (R)):: ");
    Serial.print(parameters[0]);
    Serial.print(" ");
    Serial.print(parameters[1]);
    Serial.print(" ");
    Serial.print(parameters[2]);
    Serial.println(" ");

    if (gcode_line.startsWith("G00") || gcode_line.startsWith("G0"))
    {   
        // Place the tool without engaging it!
        ;
    }
    if (gcode_line.startsWith("G01") || gcode_line.startsWith("G1"))
    {
        draw_line_mm(INT_MIN, INT_MIN, parameters[0], parameters[1]);
    }
    if (gcode_line.startsWith("G02") || gcode_line.startsWith("G2"))
    {
        draw_arc_radius_mm(INT_MIN, INT_MIN, parameters[0], parameters[1], parameters[2]);
    }
}


void add_gcode_line_to_queue(String gcode_line)
{   
    Serial.println("Adding GCODE line!: ");
    Serial.println(gcode_line);

    if (((END_GCODE_LINE_IDX + 1) % MAX_GCODE_LINES) == START_GCODE_LINE_IDX)
    {
        Serial.println("Gcode Queue full!");
        // Handling for full queue
        return;
    }

    GCODE_LINES[END_GCODE_LINE_IDX] = gcode_line;
    END_GCODE_LINE_IDX = (END_GCODE_LINE_IDX + 1) % MAX_GCODE_LINES; 
}


void read_and_execute_gcode_lines()
{
    int current_gcode_line_end_idx = END_GCODE_LINE_IDX;
    while (START_GCODE_LINE_IDX != current_gcode_line_end_idx)
    {
        interpret_gcode(GCODE_LINES[START_GCODE_LINE_IDX]);
        START_GCODE_LINE_IDX = (START_GCODE_LINE_IDX + 1) % MAX_GCODE_LINES;
    }
}