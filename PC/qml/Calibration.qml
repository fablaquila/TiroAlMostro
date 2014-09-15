// The panel for calibration. It as a back button in the central part. To
// activate a target set the status to either "up", "down", "left" or "right"
import QtQuick 2.0

AnimatedElementsPanel {
	id: container

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

	// The back button
	Button {
		id: backButton
		caption: container.backButtonCaption
		x: (container.width - width) / 2.0
		yWhenVisible: (container.height - height) / 2.0
		width: container.width * container.backButtonWidth
		height: container.height * container.backButtonHeight

		onClicked: {
			goBack();

			if (container.backItem != null) {
				container.hideAll();
			}
		}
	}

	// The up target
	CalibrationTarget {
		id: upTarget
		x: (container.width - width) / 2.0
		yWhenVisible: 0
		width: container.width * container.targetSide
		height: width
		color: container.targetOffColor
	}

	// The down target
	CalibrationTarget {
		id: downTarget
		x: (container.width - width) / 2.0
		yWhenVisible: container.height - height
		width: container.width * container.targetSide
		height: width
		color: container.targetOffColor
	}

	// The left target
	CalibrationTarget {
		id: leftTarget
		x: 0
		yWhenVisible: (container. height - height) / 2.0
		width: container.width * container.targetSide
		height: width
		color: container.targetOffColor
	}

	// The right target
	CalibrationTarget {
		id: rightTarget
		x: container.width - width
		yWhenVisible: (container. height - height) / 2.0
		width: container.width * container.targetSide
		height: width
		color: container.targetOffColor
	}

	// The animated elements
	animatedElements: [backButton, upTarget, downTarget, leftTarget, rightTarget]

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
			name: "up"

			PropertyChanges { target: upTarget; color: container.targetOnColor}
			PropertyChanges { target: downTarget; color: container.targetOffColor}
			PropertyChanges { target: leftTarget; color: container.targetOffColor}
			PropertyChanges { target: rightTarget; color: container.targetOffColor}
		},
		State {
			name: "down"

			PropertyChanges { target: upTarget; color: container.targetOffColor}
			PropertyChanges { target: downTarget; color: container.targetOnColor}
			PropertyChanges { target: leftTarget; color: container.targetOffColor}
			PropertyChanges { target: rightTarget; color: container.targetOffColor}
		},
		State {
			name: "left"

			PropertyChanges { target: upTarget; color: container.targetOffColor}
			PropertyChanges { target: downTarget; color: container.targetOffColor}
			PropertyChanges { target: leftTarget; color: container.targetOnColor}
			PropertyChanges { target: rightTarget; color: container.targetOffColor}
		},
		State {
			name: "right"

			PropertyChanges { target: upTarget; color: container.targetOffColor}
			PropertyChanges { target: downTarget; color: container.targetOffColor}
			PropertyChanges { target: leftTarget; color: container.targetOffColor}
			PropertyChanges { target: rightTarget; color: container.targetOnColor}
		}
	]

	transitions: Transition {
		from: "*"
		to: "*"

		ParallelAnimation {
			ColorAnimation { target: upTarget; duration: 250}
			ColorAnimation { target: downTarget; duration: 250}
			ColorAnimation { target: leftTarget; duration: 250}
			ColorAnimation { target: rightTarget; duration: 250}
		}
	}
}
