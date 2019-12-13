#include "NodeDataModel.hpp"

#include "StyleCollection.hpp"

using QtNodes::NodeDataModel;
using QtNodes::NodeStyle;

NodeDataModel::NodeDataModel()
	: NodeDataModel(Qt::AutoConnection)
{
  // Derived classes can initialize specific style here
}

QtNodes::NodeDataModel::NodeDataModel(Qt::ConnectionType e)
	: _nodeStyle(StyleCollection::nodeStyle())
	, mConnectionType(e)
{

}


Qt::ConnectionType QtNodes::NodeDataModel::getConnectionType() const
{
	return mConnectionType;
}

QJsonObject NodeDataModel::save() const
{
	QJsonObject modelJson;
	
	modelJson["name"] = name();
	
	return modelJson;
}


NodeStyle const& NodeDataModel::nodeStyle() const
{
	return _nodeStyle;
}


void NodeDataModel::setNodeStyle(NodeStyle const& style)
{
	_nodeStyle = style;
}
