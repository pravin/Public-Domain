using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Threading;

using BrillTagger;

namespace DocTagger
{
	public partial class WndMain : Form
	{
		const string TempPath = "C:\\doctagger.html";
		public WndMain()
		{
			InitializeComponent();
			Control.CheckForIllegalCrossThreadCalls = false;
		}

		private void btnRun_Click(object sender, EventArgs e)
		{
			Thread t = new Thread(new ThreadStart(TagIt));
			t.Name = "TagIt";
			t.Start();
		}

		private void TagIt()
		{
			btnRun.Enabled = false;

			StreamWriter swriter = new StreamWriter(TempPath, false);

			swriter.WriteLine(@"
<html>
<head>
<style>
body {
	margin:0;
	font-family: Trebuchet MS;
	font-size:10pt;
	background-color:gray;
	color:#111;
	text-align:center;
}

#content {
	margin:0 auto;
	text-align:left;
	padding:40px;
	width:600px;
	background-color:white;
}

#content h1 {
	color:#396;
	font-weight:normal;
	font-size:16pt;
}
</style>
</head>
<body><div id='content'>");

			swriter.WriteLine("<h1>Original Text</h1>");
			swriter.WriteLine(txtInput.Text.Replace("\r\n", "<br />"));

			// POS TAGGING
			sbar.Text = "POS Tagging";
			progressBar.Value = 25;
			string brillTagged = BrillTagger.BrillTagger.BrillTagged(txtInput.Text,
				true, true, true, "LEXICON.BROWN.AND.WSJ", "LEXICALRULEFILE.BROWN",
				"CONTEXTUALRULEFILE.BROWN");

			swriter.WriteLine("<h1>POS Tagged Text</h1>");
			swriter.WriteLine(brillTagged.Replace("\r\n", "<br />"));

			// Tokenizing
			sbar.Text = "Tokenizing";
			progressBar.Value = 50;
			Token[] tokens = Tokenizer.Instance.Tokenize(brillTagged);
			swriter.WriteLine("<h1>Tokenizing + Stop Word Removal</h1>");
			swriter.WriteLine("Total tokens: " + tokens.LongLength);
			//foreach (Token token in tokens)
			//{
			//    txtOutput.Text += token.Word + "\t" + token.POS + "\r\n";
			//}

			// Analyzing
			sbar.Text = "Analyzing";
			progressBar.Value = 75;
			SynSetGraph graph = new SynSetGraph(tokens);

			swriter.WriteLine("<h1>Synsets analyzed</h1>");
			swriter.WriteLine(graph.FindSynSets());
			//txtOutput.Text = graph.GetTags();

			sbar.Text = "Fetching Tags";
			progressBar.Value = 90;

			swriter.WriteLine("<h1>Selected synsets</h1>");
			swriter.WriteLine("<ul>");
			swriter.WriteLine(graph.GetTags());
			swriter.WriteLine("</ul>");

			swriter.WriteLine("<h1>Lesser synsets</h1>");
			swriter.WriteLine("<ul>");
			swriter.WriteLine(graph.GetLesserTags());
			swriter.WriteLine("</ul>");

			swriter.WriteLine("</div></body></html>");
			swriter.Flush(); swriter.Close(); swriter = null;

			webBrowser.Url = new Uri("file:///" + TempPath);

			sbar.Text = "Done!";
			progressBar.Value = 100;

			btnRun.Enabled = true;
		}

		private void OnKeyDown(object sender, KeyEventArgs e)
		{
			if (e.KeyCode == Keys.A && e.Control)
			{
				txtInput.SelectAll();
			}
		}
	}
}