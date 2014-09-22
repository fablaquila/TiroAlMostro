// The panel for joystick calibration. It as a back button and instructions in
// the central part. To activate the target set the status to "center"
import QtQuick 2.0

AnimatedElementsPanel {
	id: container

	// The width of the label with instructions as a portion of the item
	// width
	property real labelWidth: 0.9
	// The height of the label with instructions as a portion of the item
	// height
	property real labelHeight: 0.2
	// The width of the back button as a portion of the item width
	property real backButtonWidth: 0.5
	// The height of the back button as a portion of the item height
	property real backButtonHeight: 0.1
	// The caption of the back button
	property string backButtonCaption: "Indietro"
	// If this is an item, when the back button is clicked this item is
	// hidden and backItem is shown. The goBack() signal is always emitted
	property var backItem: null
	// The size of the square targets as a portion of the item width
	property real targetSide: 0.05
	// The size of the target borders as a potion of the target side
	property real targetBorder: 0.1
	// The color of the border of targets
	property color targetBorderColor: "black"
	// The color of targets when off
	property color targetOffColor: "white"
	// The color of targets when on
	property color targetOnColor: "red"

	// The signal emitted when the back button is clicked
	signal goBack()
	// The signal emitted when this panel is made visible
	signal calibrationStarted()

	// The function to call to end calibration (this simply hides this
	// panel)
	function endCalibration()
	{
		if (container.backItem != null) {
			container.hideAll();
		}
	}

	// The label describing what to do
	AnimatedLabel {
		id: label
		text: "Calibrazione del joystick. Premi un pulsante per iniziare"
		backgroundColor: "white"
		x: (container.width - width) / 2.0
		yWhenVisible: (container.height - height) / 4.0
		width: container.width * container.labelWidth
		height: container.height * container.labelHeight
	}

	// The back button
	Button {
		id: backButton
		caption: container.backButtonCaption
		x: (container.width - width) / 2.0
		yWhenVisible: 3.0 * (container.height - height) / 4.0
		width: container.width * container.backButtonWidth
		height: container.height * container.backButtonHeight

		onClicked: {
			goBack();

			if (container.backItem != null) {
				container.hideAll();
			}
		}
	}

	// The central target
	CalibrationTarget {
		id: centralTarget
		x: (container.width - width) / 2.0
		yWhenVisible: (container. height - height) / 2.0
		width: container.width * container.targetSide
		height: width
		color: container.targetOffColor
	}

	// The animated elements
	animatedElements: [label, backButton, centralTarget]

	// Called when all buttons have disappeared
	onAllDisappeared: {
		visible = false;
		if (backItem != null) {
			backItem.visible = true;
		}
	}

	// We need to reimplement this to send the calibrationStarted() signal
	// when the panel is shown
	onVisibleChanged: {
		if (visible) {
			calibrationStarted()
			showAll();
		}
	}

	states: [
		State {
			name: "center"

			PropertyChanges { target: label; text: "Centra il joystick e premi il pulsante"}
			PropertyChanges { target: centralTarget; color: container.targetOnColor}
		}
	]

	transitions: Transition {
		from: "*"
		to: "*"

		ParallelAnimation {
			ColorAnimation { target: centralTarget; duration: 250}
			SequentialAnimation {
				NumberAnimation { target: label; property: "opacity"; from: 1; to: 0; duration: 125 }
				NumberAnimation { target: label; property: "opacity"; from: 0; to: 1; duration: 125 }
			}
		}
	}
}