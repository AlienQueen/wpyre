<?php 
function removeDir($dirName)
{
    $result = false;
    if(! is_dir($dirName))
    {
        trigger_error("Ä¿Â¼Ãû³Æ´íÎó", E_USER_ERROR);
    }
    $handle = opendir($dirName);
    while(($file = readdir($handle)) !== false)
    {
        if($file != '.' && $file != '..')
        {
            $dir = $dirName . DIRECTORY_SEPARATOR . $file;
            is_dir($dir) ? removeDir($dir) : unlink($dir);
        }
    }
    closedir($handle);
    $result = rmdir($dirName) ? true : false;
    return $result;
}


if ($handle = opendir('.')) {
    while (false !== ($file = readdir($handle))) { 
        if ($file != "." && $file != "..") { 
			if(true == is_dir($file) && true==is_numeric($file))
			{
			echo "Deleting $file<br>\n";
			removeDir($file);
			}
        } 
    }
    closedir($handle); 
}
echo "Done<br>\n";
?>
 