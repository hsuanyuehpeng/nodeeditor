#pragma once

#include <QtWidgets/QWidget>

#include "PortType.hpp"
#include "NodeData.hpp"
#include "Serializable.hpp"
#include "NodeGeometry.hpp"
#include "NodeStyle.hpp"
#include "NodePainterDelegate.hpp"
#include "Export.hpp"
#include "memory.hpp"

namespace QtNodes
{

enum class NodeValidationState
{
	Valid,
	Warning,
	Error
};

class Connection;

class StyleCollection;

class NODE_EDITOR_PUBLIC NodeDataModel
  : public QObject
  , public Serializable
{
	Q_OBJECT

public:

	NodeDataModel();

	NodeDataModel(Qt::ConnectionType e);
	
	virtual ~NodeDataModel() = default;
	
	/// Caption is used in GUI
	virtual QString caption() const = 0;
	
	/// It is possible to hide caption in GUI
	virtual bool captionVisible() const { return true; }
	
	/// Port caption is used in GUI to label individual ports
	virtual QString portCaption(PortType, PortIndex) const { return QString(); }
	
	/// It is possible to hide port caption in GUI
	virtual bool portCaptionVisible(PortType, PortIndex) const { return false; }
	
	/// Name makes this model unique, this is also used in the dropdown box in GUI selection
	virtual QString name() const = 0;

	Qt::ConnectionType getConnectionType() const;

public:

	QJsonObject save() const override;

public:

	virtual unsigned int nPorts(PortType portType) const = 0;
	
	virtual NodeDataType dataType(PortType portType, PortIndex portIndex) const = 0;

public:

	enum class ConnectionPolicy
	{
		One,
		Many,
	};
	
	virtual ConnectionPolicy portOutConnectionPolicy(PortIndex) const
	{
		return ConnectionPolicy::Many;
	}
	
	NodeStyle const& nodeStyle() const;
	
	void setNodeStyle(NodeStyle const& style);

public:

	/// Triggers the algorithm
	virtual void setInData(std::shared_ptr<NodeData> nodeData,PortIndex port) = 0;
	
	virtual std::shared_ptr<NodeData> outData(PortIndex port) = 0;
	
	virtual QWidget* embeddedWidget() = 0;
	
	virtual bool resizable() const { return false; }

	virtual bool isInputValid(PortIndex port) const = 0;

	bool areAllInputsValid() const
	{
		for(PortIndex i = 0; i < nPorts(PortType::In); i++)
		{
			if(!isInputValid(i))
			{
				return false;
			}
		}
		return true;
	}
	
	// This should only verify the inputs that are necessary for outputting
	virtual bool hasValidInputsForOutputs() 
	{
		return areAllInputsValid();
	}

	virtual NodeValidationState validationState() const { return NodeValidationState::Valid; }
	
	virtual QString validationMessage() const { return QString(""); }
	
	virtual NodePainterDelegate* painterDelegate() const { return nullptr; }

public Q_SLOTS:

	virtual void
	inputConnectionCreated(Connection const&)
	{
	}
	
	virtual void
	inputConnectionDeleted(Connection const&)
	{
	}
	
	virtual void
	outputConnectionCreated(Connection const&)
	{
	}
	
	virtual void
	outputConnectionDeleted(Connection const&)
	{
	}

Q_SIGNALS:

	void dataUpdated(PortIndex index);
	
	void dataInvalidated(PortIndex index);
	
	void computingStarted();
	
	void computingFinished();
	
	void embeddedWidgetSizeUpdated();

private:

	Qt::ConnectionType mConnectionType;

	NodeStyle _nodeStyle;
};
}
