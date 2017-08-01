#include <QWidget>
#include <QXmlStreamWriter>
#include <QFile>
#include <QMessageBox>

void writeXML()
{
    QXmlStreamWriter xmlWriter;
    QFile file("output.xml");

    if (!file.open(QIODevice::WriteOnly))
    {
      QMessageBox::warning(0, "Error!", "Error opening file");
    }
    else
    {
	xmlWriter.setDevice(&file);

	/* Writes a document start with the XML version number. */
	xmlWriter.writeStartDocument();
	xmlWriter.writeStartElement("students");

	xmlWriter.writeStartElement("student");
	/*add one attribute and its value*/
	xmlWriter.writeAttribute("name","Kate");
	/*add one attribute and its value*/
	xmlWriter.writeAttribute("surname","Johns");
	/*add one attribute and its value*/
	xmlWriter.writeAttribute("number","154455");
	/*add character data to tag student */
	xmlWriter.writeCharacters ("Student 1");
	/*close tag student  */
	xmlWriter.writeEndElement();

        /*end tag students*/
        xmlWriter.writeEndElement();
        /*end document */
        xmlWriter.writeEndDocument();
   }
};

void writeSetupXML()
{
    QXmlStreamWriter xmlWriter;
    QFile file("setup.xml");

    if (!file.open(QIODevice::WriteOnly))
    {
      QMessageBox::warning(0, "Error!", "Error opening file");
    }
    else
    {
    xmlWriter.setAutoFormatting(true);
	xmlWriter.setDevice(&file);
	xmlWriter.writeStartDocument();
	xmlWriter.writeStartElement("setup");

	xmlWriter.writeStartElement("rpc");
	xmlWriter.writeAttribute("id","1");

	xmlWriter.writeStartElement("gaps");
	xmlWriter.writeCharacters("2");
	xmlWriter.writeEndElement(); //gaps

	xmlWriter.writeStartElement("roplanes");
	xmlWriter.writeCharacters("2");
	xmlWriter.writeEndElement(); //roplanes

	xmlWriter.writeStartElement("plane");
	xmlWriter.writeAttribute("id","1");
	xmlWriter.writeStartElement("ps");
	xmlWriter.writeStartElement("slot");
	xmlWriter.writeEndElement(); //slot
	xmlWriter.writeStartElement("channel");
	xmlWriter.writeEndElement(); //channel
	xmlWriter.writeEndElement(); //ps

	xmlWriter.writeStartElement("tdc");
	xmlWriter.writeAttribute("id","2");
	xmlWriter.writeStartElement("channelStart");
	xmlWriter.writeCharacters("32");
	xmlWriter.writeEndElement(); //channelStart
	xmlWriter.writeStartElement("channelEnd");
	xmlWriter.writeCharacters("63");
	xmlWriter.writeEndElement(); //channelEnd
	xmlWriter.writeEndElement(); //tdc

	xmlWriter.writeEndElement(); //plane


	xmlWriter.writeStartElement("plane");
	xmlWriter.writeAttribute("id","2");
		xmlWriter.writeStartElement("ps");
		xmlWriter.writeStartElement("slot");
		xmlWriter.writeEndElement(); //slot
		xmlWriter.writeStartElement("channel");
		xmlWriter.writeEndElement(); //channel
		xmlWriter.writeEndElement(); //ps

		xmlWriter.writeStartElement("tdc");
		xmlWriter.writeAttribute("id","2");
		xmlWriter.writeStartElement("channelStart");
		xmlWriter.writeCharacters("64");
		xmlWriter.writeEndElement(); //channelStart
		xmlWriter.writeStartElement("channelEnd");
		xmlWriter.writeCharacters("95");
		xmlWriter.writeEndElement(); //channelEnd
		xmlWriter.writeEndElement(); //tdc

		xmlWriter.writeEndElement(); //plane



	xmlWriter.writeEndElement();//rpc
	xmlWriter.writeEndElement();//setup
	xmlWriter.writeEndDocument();

    }
}
int main(int argc, char* argv[])
{
  writeXML();
  writeSetupXML();
}

