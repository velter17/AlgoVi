/**
 * Project   AlgoVi
 *
 * @file     CSettingsReader.cpp
 * @author   Dmytro Sadovyi
 * @date     18.05.2017
 */

#include <QFile>
#include <QtXml/QtXml>

#include "framework/settings/readers/CSettingsReader.hpp"

namespace
{

QString removeExtraSpaces(const QString& str)
{
   QString ret = str;
   while(ret.endsWith(QChar::Space) || ret.endsWith(QChar::Tabulation) || ret.endsWith('\n'))
   {
      ret.chop(1);
   }
   int idx = 0;
   while(idx < ret.length() && (ret[idx] == QChar::Space || ret[idx] == QChar::Tabulation || ret[idx].toLatin1() == '\n'))
   {
      ++idx;
   }
   return ret.mid(idx);
}

} // anonymous namespace

namespace NSettings
{

CSettingsReader::CSettingsReader(const QString &configPath)
   : mConfigPath(configPath)
{

}

QVector<CSettingsData> CSettingsReader::readConfig()
{
   QVector <CSettingsData> ret;
   QFile file(mConfigPath);
   if(!file.open(QFile::ReadOnly | QFile::Text))
   {
      throw QString("failed to open " + mConfigPath);
   }
   QDomDocument document;
   document.setContent(&file);
   QDomNode node = document.firstChildElement().firstChild();
   while(!node.isNull())
   {
      if(node.isComment())
      {
         node = node.nextSibling();
         continue;
      }
      if(!node.isElement())
      {
         throw QString("invalid config structure");
      }
      QDomElement elem = node.toElement();
      tAttributeMap attrs;
      for(int i = 0; i < elem.attributes().length(); ++i)
      {
         QDomAttr x = elem.attributes().item(i).toAttr();
         attrs.insert(x.name(), removeExtraSpaces(x.value()));
      }
      CSettingsData data;
      data.setName(elem.tagName());
      data.setValue(removeExtraSpaces(elem.text()));
      data.setAttributes(attrs);
      ret.push_back(data);

      node = node.nextSibling();
   }
   return ret;
}

} // namespace NSettings
