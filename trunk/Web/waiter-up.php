<?php
$id=$_GET['id'];

if (move_uploaded_file($_FILES['userfile']['tmp_name'], $id."/".$_FILES['userfile']['name'])) {
    print "OK\nFile is valid, and was successfully uploaded.  Here's some more debugging info:\n";
    print_r($_FILES);
	

//	rename($_FILES['userfile']['name'], $id."/".$_FILES['userfile']['name']);
///////////////////////////////////////////////////////////
	$sign=0;
	$handle = fopen ($id."/upload.txt", "r+");
	if(!$handle)
	{
	$handle = fopen ($id."/upload.txt", "w+");
	fclose($handle);
	$handle = fopen ($id."/upload.txt", "r+");
	}
	while(!feof($handle))
	{
		if(strstr(fgets($handle, 128),$_FILES['userfile']['name']))
		{
			$sign=1;
			break;
		}
	}

	if($sign==0)
	{
		if($handle==FALSE)
		{
			printf("Creating Failed!<br>\n");
			exit();
		}
//		fprintf($handle, "%s\n", $_FILES['userfile']['name']);
		fputs($handle, sprintf("%s\n", $_FILES['userfile']['name']));
	}
	fclose($handle);
	///////////////////////////////////////////////////////////
	$handle = fopen($id."/list.txt", "r+");
	if(!$handle)
	{
	$handle = fopen ($id."/list.txt", "w+");
	fclose($handle);
	$handle = fopen ($id."/list.txt", "r+");
	}
	while(!feof($handle))
	{
		$locate=ftell($handle);

		$filename=$_FILES['userfile']['name'];
		$filename=substr($filename, 0, strlen($filename)-strlen(strrchr($filename, '.')));

		if(strstr(fgets($handle, 128),$filename))
		{
			fseek($handle, $locate,SEEK_SET);
//			fprintf($handle, "1000");
			fputs($handle, "1000");
			break;
		}
	}
} else {
    print "ERROR\nPossible file upload attack!  Here's some debugging info:\n";
    print_r($_FILES);
}

?>