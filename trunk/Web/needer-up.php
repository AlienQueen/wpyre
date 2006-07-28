<?php
if($_GET['id'])//upload result
{
	if(move_uploaded_file($_FILES['userfile']['tmp_name'], $_GET['id']."/".$_FILES['userfile']['name']))
	{
		printf ("OK\n");
		printf ("%s\n", $dirname);
		printf ("File is valid, and was successfully uploaded.  Here's some more debugging info:\n");
		print_r($_FILES);
	}
	else
	{
		print "ERROR\nPossible file upload attack!  Here's some debugging info:\n";
		print_r($_FILES);
	}
}
else//upload source
{
	if(move_uploaded_file($_FILES['userfile']['tmp_name'], $_FILES['userfile']['name']))
	{
		//$dirname=rand(10000000,99999999);
		while(is_dir($dirname=rand(10000000,99999999)));
		mkdir($dirname, 0777);
		rename($_FILES['userfile']['name'], $dirname."/".$_FILES['userfile']['name']);
		printf ("OK\n");
		printf ("%s\n", $dirname);
		printf ("File is valid, and was successfully uploaded.  Here's some more debugging info:\n");
		print_r($_FILES);

		$resx=$_GET['resx'];
		$resy=$_GET['resy'];

		$handle = fopen ($dirname."/conf.txt", "w+");
		if($handle!=FALSE)
		{
			fputs($handle, sprintf("%d %d ", $resx, $resy));
			fclose($handle);
		}
		else 
			exit();

	}
	else
	{
		print "ERROR\nPossible file upload attack!  Here's some debugging info:\n";
		print_r($_FILES);
	}
}
?>