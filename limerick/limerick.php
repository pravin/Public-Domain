<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head profile="http://gmpg.org/xfn/11">
	<title>Limerick Generator</title>
	<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
	<style>
body {
	margin:0; padding:0;
	background-image:url('http://blogs.rediff.com/scripts/getimage.phtml?image=1158214597.gif&imagetype=image/gif&handle=oh_its_dee@yahoo.co.in');
	background-color:#013;
	text-align:center;
}

body, td {
	font-size:10pt;
	font-family: "Lucida Grande", "Lucida Sans Unicode", Verdana, sans-serif;
	line-height:150%;
}

.wrapper {
	width:750px;
	margin:auto;
	text-align:left;
	border:0;
	border-left:4px #346 solid;
	border-right:4px #346 solid;
}

.header {
	font-family:'Trebuchet MS', sans-serif;
	font-weight:bold;
	background-color:#000;
}

.header a {
	text-decoration:none;
}

/* Top navigation menu */
.menu {
	margin:0;
	padding:10px;
	text-align:right;
}

.menu li {
	display:inline;
	padding:10px;
	margin:0;
}

.menu li a {
	text-decoration:none;
	color:#ccc;
}
.menu li a:hover {
	text-decoration:none;
	color:#9cf;
}

/* Content pane */
.content {
	background-color:#fff;
	color:#333;
	clear:both;
}

.content h1,h2,h3,h4 {
	color:#f60;
	font-family: 'Trebuchet MS', sans-serif;
}

.content h1 {
	font-size:16pt;
}

.content h1 a {
	color:#f60;
	text-decoration:none;
}

.content h1 a:hover {
	text-decoration:underline;
}

.content h2 {
	font-size:14pt;
}

.content h3 {
	font-size:12pt;
}

.content h4 {
	font-size:10pt;
}

.post {
	border:0;
	padding:20px;
	border-bottom: 1px solid #ccc;
}

.post-date {
	font-size:8pt;
	font-weight:bold;
	color:#666;
}

/* -- Footer -- */
.footer-wrapper {
	clear:both;
	float:left;
}
.footer {
	background-color:#111;
	color:#999;
	text-align:center;
	font-size:8pt;
	padding:4px;
}
.footer a {
	color:#999;
	text-decoration:none;
	border:0px;
	border-bottom: 1px #999 solid;
}
.footer img {
	border:0;
	vertical-align:middle;
}

/* Misc */
.center-me {
	text-align:center;
}

.pad-me {
	padding-top:0px;
	padding-left:20px;
	padding-right:20px;
	padding-bottom:20px;
}
.info {
	padding:20px;
	background-color:#ff9;
	border:0;
	border-top:1px #c90 solid;
	border-bottom:1px #c90 solid;
}
.quote {
	color:#606;
	padding:20px;
}

pre {
	font-size:8pt;
	line-height:150%;
}

/* -- syntax highlighting -- */

.php .kw1 {color: #b1b100;}
.php .kw2 {color: #000000; font-weight: bold;}
.php .kw3 {color: #000066;}
.php .co1 {color: #808080; font-style: italic;font-style: normal;}
.php .co2 {color: #808080; font-style: italic;}
.php .coMULTI {color: #808080; font-style: italic;font-style: normal;}
.php .es0 {color: #000099; font-weight: bold;font-weight: normal;}
.php .br0 {color: #66cc66;}
.php .st0 {color: #ff0000;}
.php .nu0 {color: #cc66cc;}
.php .me1 {color: #006600;}
.php .me2 {color: #006600;}
.php .me {1}
.php .re0 {color: #0000ff;}
.php .re1 {color: #ff0000}

	</style>

<body>
<div class="wrapper">
	<!-- Header -->
	<div class="header">

	<div style="font-size:18pt; padding: 5px 0 0 15px;"><a href="http://www.dustyant.com"><span style="color:#9f9" title="Pravin Paratey">Dusty</span><span style="color:#99f" title="Pravin Paratey">Ant</span></a></div>
	<ul class="menu"></ul>
	</div>				
<?php
	
	/// Generates a word with $num_consonants number of consonants
	/// ex. $num_consonants = 3 will produce words like:
	/// - diwali
	/// - beaten
	function generate_word($num_consonants)
	{
		$output = ""; // Return variable
		$vowels = array('a', 'e', 'i', 'o', 'u');
		
		for($i = 0; $i < $num_consonants; $i++)
		{
			// Pick a random character and a vowel
			$char = 98;
			do
			{
				$char = rand(98, 122); // b - z
			}
			while($char == 101 || $char == 105 || $char == 111 || $char == 117);
			$vowel_selector = rand(0, 4);
			
			// At this point, we've got char and a vowel
			// Now we decide which one goes first
			$toss = rand(0, 1);
			if ($toss == 1)
			{
				$output = $output . chr($char) . $vowels[$vowel_selector];
			}
			else
			{
				$output = $output . $vowels[$vowel_selector] . chr($char);
			}
		}
		
		return $output;
	}

	/// Generates a sentence with $num_words number of words
	function generate_sentence($num_words)
	{
		$output = '';
		for($i = 0; $i < $num_words; $i++)
		{
			$output = $output . generate_word(rand(1,3)) . ' ';
		}
		
		return $output;
	}
	
	
	/// Replaces the last two characters of $sentence with $suffix
	function replace_sentence($sentence, $suffix)
	{
		return substr($sentence, 0, strlen($sentence) - 2) . $suffix;
	}

	/// Returns a randomly generated alien limerick
	function generate_limerick()
	{
		$first_line = generate_sentence(6);
		$suffix = substr($first_line, strlen($first_line) - 3);
		
		$second_line = replace_sentence(generate_sentence(6), $suffix);
		$fifth_line = replace_sentence(generate_sentence(6), $suffix);
		
		$third_line = generate_sentence(3);
		$suffix = substr($third_line, strlen($third_line) - 3);
		$fourth_line = replace_sentence(generate_sentence(3), $suffix);
		
		
		$output = $first_line . '<br />' . 
			$second_line . '<br />' .
			$third_line . '<br />' .
			$fourth_line . '<br />' .
			$fifth_line . '<br />';
			
		return $output;
	}
?>


	<!-- Content -->
	<div class="content">
		<div class="post">
		<h1>Limerick Generator - Part I</h1>
			<div class="info">
				<?php echo generate_limerick(); ?>
			</div>
			Press F5 or refresh this page to see a new <a href="http://www.dustyant.com/posts/limerick-generator-part-i.html">alien limerick</a>.

			<h2>Code</h2>
<div class="php" style="white-space: nowrap;border: 1px dotted #a0a0a0; font-family: 'Courier New', Courier, monospace; font-size: 8pt; background-color:#fcfcfc; margin: 0; line-height: 110%; padding: 0;color: #000000;">
<span class="kw2">&lt;?php</span><br />
&nbsp; &nbsp; <br />
&nbsp; &nbsp; <span class="co1">/// Generates a word with $num_consonants number of consonants</span><br />
&nbsp; &nbsp; <span class="co1">/// ex. $num_consonants = 3 will produce words like:</span><br />
&nbsp; &nbsp; <span class="co1">/// - diwali</span><br />

&nbsp; &nbsp; <span class="co1">/// - beaten</span><br />
&nbsp; &nbsp; <span class="kw2">function</span> generate_word<span class="br0">&#40;</span><span class="re0">$num_consonants</span><span class="br0">&#41;</span><br />
&nbsp; &nbsp; <span class="br0">&#123;</span><br />
&nbsp; &nbsp; &nbsp; &nbsp; <span class="re0">$output</span> = <span class="st0">&quot;&quot;</span>; <span class="co1">// Return variable</span><br />

&nbsp; &nbsp; &nbsp; &nbsp; <span class="re0">$vowels</span> = <a href="http://www.php.net/array"><span class="kw3">array</span></a><span class="br0">&#40;</span><span class="st0">'a'</span>, <span class="st0">'e'</span>, <span class="st0">'i'</span>, <span class="st0">'o'</span>, <span class="st0">'u'</span><span class="br0">&#41;</span>;<br />

&nbsp; &nbsp; &nbsp; &nbsp; <br />
&nbsp; &nbsp; &nbsp; &nbsp; <span class="kw1">for</span><span class="br0">&#40;</span><span class="re0">$i</span> = <span class="nu0">0</span>; <span class="re0">$i</span> &lt; <span class="re0">$num_consonants</span>; <span class="re0">$i</span>++<span class="br0">&#41;</span><br />

&nbsp; &nbsp; &nbsp; &nbsp; <span class="br0">&#123;</span><br />
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <span class="co1">// Pick a random character and a vowel</span><br />
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <span class="re0">$char</span> = <span class="nu0">98</span>;<br />

&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <span class="kw1">do</span><br />
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <span class="br0">&#123;</span><br />
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <span class="re0">$char</span> = <a href="http://www.php.net/rand"><span class="kw3">rand</span></a><span class="br0">&#40;</span><span class="nu0">98</span>, <span class="nu0">122</span><span class="br0">&#41;</span>; <span class="co1">// b - z</span><br />

&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <span class="br0">&#125;</span><br />
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <span class="kw1">while</span><span class="br0">&#40;</span><span class="re0">$char</span> == <span class="nu0">101</span> || <span class="re0">$char</span> == <span class="nu0">105</span> || <span class="re0">$char</span> == <span class="nu0">111</span> || <span class="re0">$char</span> == <span class="nu0">117</span><span class="br0">&#41;</span>;<br />

&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <span class="re0">$vowel_selector</span> = <a href="http://www.php.net/rand"><span class="kw3">rand</span></a><span class="br0">&#40;</span><span class="nu0">0</span>, <span class="nu0">4</span><span class="br0">&#41;</span>;<br />
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <br />

&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <span class="co1">// At this point, we've got char and a vowel</span><br />
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <span class="co1">// Now we decide which one goes first</span><br />
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <span class="re0">$toss</span> = <a href="http://www.php.net/rand"><span class="kw3">rand</span></a><span class="br0">&#40;</span><span class="nu0">0</span>, <span class="nu0">1</span><span class="br0">&#41;</span>;<br />

&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <span class="kw1">if</span> <span class="br0">&#40;</span><span class="re0">$toss</span> == <span class="nu0">1</span><span class="br0">&#41;</span><br />
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <span class="br0">&#123;</span><br />

&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <span class="re0">$output</span> = <span class="re0">$output</span> . <a href="http://www.php.net/chr"><span class="kw3">chr</span></a><span class="br0">&#40;</span><span class="re0">$char</span><span class="br0">&#41;</span> . <span class="re0">$vowels</span><span class="br0">&#91;</span><span class="re0">$vowel_selector</span><span class="br0">&#93;</span>;<br />

&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <span class="br0">&#125;</span><br />
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <span class="kw1">else</span><br />
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <span class="br0">&#123;</span><br />

&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <span class="re0">$output</span> = <span class="re0">$output</span> . <span class="re0">$vowels</span><span class="br0">&#91;</span><span class="re0">$vowel_selector</span><span class="br0">&#93;</span> . <a href="http://www.php.net/chr"><span class="kw3">chr</span></a><span class="br0">&#40;</span><span class="re0">$char</span><span class="br0">&#41;</span>;<br />

&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <span class="br0">&#125;</span><br />
&nbsp; &nbsp; &nbsp; &nbsp; <span class="br0">&#125;</span><br />
&nbsp; &nbsp; &nbsp; &nbsp; <br />

&nbsp; &nbsp; &nbsp; &nbsp; <span class="kw1">return</span> <span class="re0">$output</span>;<br />
&nbsp; &nbsp; <span class="br0">&#125;</span><br />
<br />
&nbsp; &nbsp; <span class="co1">/// Generates a sentence with $num_words number of words</span><br />

&nbsp; &nbsp; <span class="kw2">function</span> generate_sentence<span class="br0">&#40;</span><span class="re0">$num_words</span><span class="br0">&#41;</span><br />
&nbsp; &nbsp; <span class="br0">&#123;</span><br />
&nbsp; &nbsp; &nbsp; &nbsp; <span class="re0">$output</span> = <span class="st0">''</span>;<br />

&nbsp; &nbsp; &nbsp; &nbsp; <span class="kw1">for</span><span class="br0">&#40;</span><span class="re0">$i</span> = <span class="nu0">0</span>; <span class="re0">$i</span> &lt; <span class="re0">$num_words</span>; <span class="re0">$i</span>++<span class="br0">&#41;</span><br />

&nbsp; &nbsp; &nbsp; &nbsp; <span class="br0">&#123;</span><br />
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <span class="re0">$output</span> = <span class="re0">$output</span> . generate_word<span class="br0">&#40;</span><a href="http://www.php.net/rand"><span class="kw3">rand</span></a><span class="br0">&#40;</span><span class="nu0">1</span>,<span class="nu0">3</span><span class="br0">&#41;</span><span class="br0">&#41;</span> . <span class="st0">' '</span>;<br />

&nbsp; &nbsp; &nbsp; &nbsp; <span class="br0">&#125;</span><br />
&nbsp; &nbsp; &nbsp; &nbsp; <br />
&nbsp; &nbsp; &nbsp; &nbsp; <span class="kw1">return</span> <span class="re0">$output</span>;<br />

&nbsp; &nbsp; <span class="br0">&#125;</span><br />
&nbsp; &nbsp; <br />
&nbsp; &nbsp; <br />
&nbsp; &nbsp; <span class="co1">/// Replaces the last two characters of $sentence with $suffix</span><br />
&nbsp; &nbsp; <span class="kw2">function</span> replace_sentence<span class="br0">&#40;</span><span class="re0">$sentence</span>, <span class="re0">$suffix</span><span class="br0">&#41;</span><br />

&nbsp; &nbsp; <span class="br0">&#123;</span><br />
&nbsp; &nbsp; &nbsp; &nbsp; <span class="kw1">return</span> <a href="http://www.php.net/substr"><span class="kw3">substr</span></a><span class="br0">&#40;</span><span class="re0">$sentence</span>, <span class="nu0">0</span>, <a href="http://www.php.net/strlen"><span class="kw3">strlen</span></a><span class="br0">&#40;</span><span class="re0">$sentence</span><span class="br0">&#41;</span> - <span class="nu0">2</span><span class="br0">&#41;</span> . <span class="re0">$suffix</span>;<br />

&nbsp; &nbsp; <span class="br0">&#125;</span><br />
<br />
&nbsp; &nbsp; <span class="co1">/// Returns a randomly generated alien limerick</span><br />
&nbsp; &nbsp; <span class="kw2">function</span> generate_limerick<span class="br0">&#40;</span><span class="br0">&#41;</span><br />
&nbsp; &nbsp; <span class="br0">&#123;</span><br />

&nbsp; &nbsp; &nbsp; &nbsp; <span class="re0">$first_line</span> = generate_sentence<span class="br0">&#40;</span><span class="nu0">6</span><span class="br0">&#41;</span>;<br />
&nbsp; &nbsp; &nbsp; &nbsp; <span class="re0">$suffix</span> = <a href="http://www.php.net/substr"><span class="kw3">substr</span></a><span class="br0">&#40;</span><span class="re0">$first_line</span>, <a href="http://www.php.net/strlen"><span class="kw3">strlen</span></a><span class="br0">&#40;</span><span class="re0">$first_line</span><span class="br0">&#41;</span> - <span class="nu0">3</span><span class="br0">&#41;</span>;<br />

&nbsp; &nbsp; &nbsp; &nbsp; <br />
&nbsp; &nbsp; &nbsp; &nbsp; <span class="re0">$second_line</span> = replace_sentence<span class="br0">&#40;</span>generate_sentence<span class="br0">&#40;</span><span class="nu0">6</span><span class="br0">&#41;</span>, <span class="re0">$suffix</span><span class="br0">&#41;</span>;<br />

&nbsp; &nbsp; &nbsp; &nbsp; <span class="re0">$fifth_line</span> = replace_sentence<span class="br0">&#40;</span>generate_sentence<span class="br0">&#40;</span><span class="nu0">6</span><span class="br0">&#41;</span>, <span class="re0">$suffix</span><span class="br0">&#41;</span>;<br />
&nbsp; &nbsp; &nbsp; &nbsp; <br />

&nbsp; &nbsp; &nbsp; &nbsp; <span class="re0">$third_line</span> = generate_sentence<span class="br0">&#40;</span><span class="nu0">3</span><span class="br0">&#41;</span>;<br />
&nbsp; &nbsp; &nbsp; &nbsp; <span class="re0">$suffix</span> = <a href="http://www.php.net/substr"><span class="kw3">substr</span></a><span class="br0">&#40;</span><span class="re0">$third_line</span>, <a href="http://www.php.net/strlen"><span class="kw3">strlen</span></a><span class="br0">&#40;</span><span class="re0">$third_line</span><span class="br0">&#41;</span> - <span class="nu0">3</span><span class="br0">&#41;</span>;<br />

&nbsp; &nbsp; &nbsp; &nbsp; <span class="re0">$fourth_line</span> = replace_sentence<span class="br0">&#40;</span>generate_sentence<span class="br0">&#40;</span><span class="nu0">3</span><span class="br0">&#41;</span>, <span class="re0">$suffix</span><span class="br0">&#41;</span>;<br />
&nbsp; &nbsp; &nbsp; &nbsp; <br />

&nbsp; &nbsp; &nbsp; &nbsp; <br />
&nbsp; &nbsp; &nbsp; &nbsp; <span class="re0">$output</span> = <span class="re0">$first_line</span> . <span class="st0">'&lt;br /&gt;'</span> . <br />

&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <span class="re0">$second_line</span> . <span class="st0">'&lt;br /&gt;'</span> .<br />
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <span class="re0">$third_line</span> . <span class="st0">'&lt;br /&gt;'</span> .<br />

&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <span class="re0">$fourth_line</span> . <span class="st0">'&lt;br /&gt;'</span> .<br />
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <span class="re0">$fifth_line</span> . <span class="st0">'&lt;br /&gt;'</span>;<br />

&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <br />
&nbsp; &nbsp; &nbsp; &nbsp; <span class="kw1">return</span> <span class="re0">$output</span>;<br />
&nbsp; &nbsp; <span class="br0">&#125;</span><br />

<span class="kw2">?&gt;</span></div>
<div class="head">Syntax coloring via GeSHi &copy; 2004, Nigel McNie</div>
		</div>		
	</div>




	<!-- End Content -->
	<!-- Footer -->
	<div class="footer">
		This page and all its contents Copyright © <a href="http://www.dustyant.com" title="Dusty Ant">Pravin Paratey</a> 2001-2006.
		<!-- Start of StatCounter Code -->

		<script type="text/javascript">
		<!-- 
		var sc_project=334381; 
		var sc_invisible=1; 
		var sc_partition=1; 
		var sc_security=""; 
		//-->
		</script>

		<script type="text/javascript" src="http://www.statcounter.com/counter/counter_xhtml.js"></script><noscript><div class="statcounter"><a class="statcounter" href="http://www.statcounter.com/"><img class="statcounter" src="http://c2.statcounter.com/counter.php?sc_project=334381&java=0&security=&invisible=1" alt="web stats" /></a></div></noscript>
		<!-- End of StatCounter Code -->
	</div>
</div>
</body>
</html>