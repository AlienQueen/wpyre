<?php
$id = $_GET['id'];
// printf("id=%s",$id);
// exit();
$handle = fopen ("conf.txt", "r+");
fscanf($handle, "%d %d", $grid_x, $grid_y);
fclose($handle);
$handle = @fopen ($id . "/process.txt", "r+");
if($handle == FALSE)
{
     $handle = fopen ($id . "/process.txt", "w+");
     if($handle == FALSE)
    {
         exit();
         }
    // fprintf($handle, "%d %d ", 0, 1);
    fputs($handle, sprintf("%d %d ", 0, 1));
    
     rewind($handle);
    }
fscanf($handle, "%d %d", $process_x, $process_y);

// if file does not exist
if($process_y == 0)$process_y = 1;

$process_x++;
if($process_x > $grid_x)
{
     if(($process_x >= $grid_x) && ($process_y >= $grid_y))
        {
         fclose($handle);
         $handle = fopen($id . "/list.txt", "r+");
         $lowest = 1000;
         while(!feof($handle))
        {
             $location = ftell($handle);
             $buf = fgets($handle, 128);
             sscanf($buf, "%d", $num);
             if($num < $lowest)
            {
                 $lowest = $num;
                 $location_lowest = $location;
                 }
             }
         if($lowest < 1000)
        {
             fseek($handle, $location_lowest, SEEK_SET);
            // fprintf($handle, "%d", $lowest+1);
            fputs($handle, sprintf("%d", $lowest + 1));
            
             $buf = trim(fgets($handle, 128));
            
             $pieces = explode(" ~ ", $buf);
             printf("%s\n<br>", $pieces[0]);
             $buf = $pieces[1];
             $pieces = explode(": ", $buf);
             printf("Command Line: \n%s\n<br>\n", $pieces[1]);
            
            // printf("%d,%d-%d,%d\n<br>", round($pixelx0)+1, round($pixely0)+1, round($pixelx1), round($pixely1));
            // printf("Command Line: \nyafray -r %f:%f:%f:%f source.xml\n<br>\n", $x0, $x1, -$y0, -$y1);
        }
        else
             printf("ERR: The task has been finished.");
        
         fclose($handle);
         exit();
         }
     $process_x = 1;
     $process_y++;
    }
rewind($handle);
// printf("Grid.x: %d  Grid.Y: %d\n<br>\n", $grid_x, $grid_y);
// fprintf($handle, "%d %d ", $process_x, $process_y);
fputs($handle, sprintf("%d %d ", $process_x, $process_y));
fclose($handle);
// printf("Process.x: %d  Process.Y: %d<br>\n", $process_x, $process_y);
$x0 = -1 + (($process_x-1) * (2 / $grid_x));
$x1 = -1 + (($process_x-1) * (2 / $grid_x)) + 0.2;
$y0 = -1 + (($process_y-1) * (2 / $grid_y)) + 0.2;
$y1 = -1 + (($process_y-1) * (2 / $grid_y));
// printf("Command Line: yafray -r %f:%f:%f:%f source.xml<br>\n", $x0, $x1, -$y0, -$y1);
$pixelx0 = 1024 * (($x0 + 1) / 2);
$pixelx1 = 1024 * (($x1 + 1) / 2);
$pixely0 = 768 * (($y1 + 1) / 2);
$pixely1 = 768 * (($y0 + 1) / 2);

if(round($pixelx0) + 1 == 1 && round($pixely0) + 1 == 1)$handle = fopen ($id . "/list.txt", "w+");
$handle = fopen ($id . "/list.txt", "a+");
// fprintf($handle, "0    %d,%d-%d,%d ~ ", round($pixelx0)+1, round($pixely0)+1, round($pixelx1), round($pixely1));
fputs($handle, sprintf("0    %d,%d-%d,%d ~ ", round($pixelx0) + 1, round($pixely0) + 1, round($pixelx1), round($pixely1)));

// fprintf($handle, "Command Line: yafray -r %f:%f:%f:%f source.xml\n", $x0, $x1, -$y0, -$y1);
fputs($handle, sprintf("Command Line: yafray -r %f:%f:%f:%f source.xml\n", $x0, $x1, - $y0, - $y1));
fclose($handle);

printf("%d,%d-%d,%d\n<br>", round($pixelx0) + 1, round($pixely0) + 1, round($pixelx1), round($pixely1));
printf("Command Line: \nyafray -r %f:%f:%f:%f source.xml\n<br>\n", $x0, $x1, - $y0, - $y1);
printf("From (%f, %f) to (%f, %f)<br>\n", $pixelx0, $pixely0, $pixelx1, $pixely1);
printf("From (%d, %d) to (%d, %d)<br>\n", round($pixelx0) + 1, round($pixely0) + 1, round($pixelx1), round($pixely1));
printf("Grid.x: %d  Grid.Y: %d\n<br>\n", $grid_x, $grid_y);
printf("Process.x: %d  Process.Y: %d<br>\n", $process_x, $process_y);

printf("<table border=\"0\" cellspacing=\"1\" cellpadding=\"0\">\n");
for($i = 1;$i <= $grid_y;$i++)
{
     printf("\t<tr>\n");
     for($j = 1;$j <= $grid_x;$j++)
    {
         if($i < $process_y || ($j < $process_x && $i == $process_y)) // yellow
            printf("\t\t<td bgcolor=\"#FFFF00\"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </td>\n");
         elseif($j == $process_x && $i == $process_y) // red
            printf("\t\t<td bgcolor=\"#FF0000\"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </td>\n");
         else // gray
            printf("\t\t<td bgcolor=\"#808080\"> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </td>\n");
         }
     printf("\t</tr>\n");
    }
printf("</table>");
?>
