import sys, thread, time, math

sys.path.append("../lib")
print(sys.path)
import Leap
from Leap import CircleGesture, KeyTapGesture, ScreenTapGesture, SwipeGesture
from Client import NetClient


class DirectionListener(Leap.Listener):
    state_names = ['STATE_INVALID', 'STATE_START', 'STATE_UPDATE', 'STATE_END']
    finger_names = ['Thumb', 'Index', 'Middle', 'Ring', 'Pinky']
    bone_names = ['Metacarpal', 'Proximal', 'Intermediate', 'Distal']

    c = NetClient()
    g_timestap = 0

    def on_init(self, controller):
        print "Initialized"

    def on_connect(self, controller):
        print "Connected"
        frame = controller.frame()
        self.g_timestap = frame.timestamp

        # Enable gestures
        controller.enable_gesture(Leap.Gesture.TYPE_CIRCLE)
        controller.enable_gesture(Leap.Gesture.TYPE_KEY_TAP)
        controller.enable_gesture(Leap.Gesture.TYPE_SCREEN_TAP)
        controller.enable_gesture(Leap.Gesture.TYPE_SWIPE)

    def on_frame(self, controller):

        frame = controller.frame()

        if (frame.timestamp >= self.g_timestap + 100000):
            self.g_timestap = frame.timestamp
            command = self.get_hands_info(controller, frame)
            if command != None :
                self.c.sendTo(bytes(command))
            # self.get_hands_info(controller, frame)


        # print "Frame id: %d, timestamp: %d, hands: %d, fingers: %d, tools: %d, gestures: %d" % (
        #     frame.id, frame.timestamp, len(frame.hands), len(frame.fingers), len(frame.tools), len(frame.gestures()))

        # self.get_gesture(controller, frame)
        # self.get_frame_info(controller, frame)
        #     self.get_hands_info(controller, frame)

    def get_hands_info(self, controller, frame):
        # Get hands
        for hand in frame.hands:

            handType = "Left hand" if hand.is_left else "Right hand"

            # print "  %s, id %d, position: %s" % (
            #     handType, hand.id, hand.palm_position)

            pos = hand.palm_position

            x_pos = pos[0]
            y_pos = pos[2]

            if (abs(x_pos) <= 50.0) & (abs(y_pos) <= 40.0) :
                return 0
            elif abs(x_pos) > 50.0:
                if x_pos >= 0:
                    return 3
                else:
                    return 4
            else:
                if y_pos <= 0:
                    return 1
                else:
                    return 2



            # # Get the hand's normal vector and direction
            # normal = hand.palm_normal
            # direction = hand.direction
            #
            # # Calculate the hand's pitch, roll, and yaw angles
            # print "  pitch: %f degrees, roll: %f degrees, yaw: %f degrees" % (
            #     direction.pitch * Leap.RAD_TO_DEG,
            #     normal.roll * Leap.RAD_TO_DEG,
            #     direction.yaw * Leap.RAD_TO_DEG)

            # Get arm bone
            # arm = hand.arm
            # print "  Arm direction: %s, wrist position: %s, elbow position: %s" % (
            #     arm.direction,
            #     arm.wrist_position,
            #     arm.elbow_position)

            # Get fingers
            # for finger in hand.fingers:
            #
            #     print "    %s finger, id: %d, length: %fmm, width: %fmm" % (
            #         self.finger_names[finger.type],
            #         finger.id,
            #         finger.length,
            #         finger.width)
            #
            #     # Get bones
            #     for b in range(0, 4):
            #         bone = finger.bone(b)
            #         print "      Bone: %s, start: %s, end: %s, direction: %s" % (
            #             self.bone_names[bone.type],
            #             bone.prev_joint,
            #             bone.next_joint,
            #             bone.direction)

    def get_frame_info(self, controller, frame):
        print "Frame id: %d, timestamp: %d, hands: %d, fingers: %d, tools: %d, gestures: %d" % (
            frame.id, frame.timestamp, len(frame.hands), len(frame.fingers), len(frame.tools), len(frame.gestures()))

    def get_gesture(self, controller, frame):
        # Get gestures
        for gesture in frame.gestures():
            if gesture.type == Leap.Gesture.TYPE_CIRCLE:
                circle = CircleGesture(
                    gesture)  # Determine clock direction using the angle between the pointable and the circle normal
                if circle.pointable.direction.angle_to(circle.normal) <= Leap.PI / 2:
                    clockwiseness = "clockwise"
                else:
                    clockwiseness = "counterclockwise"

                # Calculate the angle swept since the last frame
                swept_angle = 0
                if circle.state != Leap.Gesture.STATE_START:
                    previous_update = CircleGesture(controller.frame(1).gesture(circle.id))
                    swept_angle = (circle.progress - previous_update.progress) * 2 * Leap.PI

                print "  Circle id: %d, %s, progress: %f, radius: %f, angle: %f degrees, %s" % (
                    gesture.id, self.state_names[gesture.state],
                    circle.progress, circle.radius, swept_angle * Leap.RAD_TO_DEG, clockwiseness)

            if gesture.type == Leap.Gesture.TYPE_SWIPE:
                swipe = SwipeGesture(gesture)
                print "  Swipe id: %d, state: %s, position: %s, direction: %s, speed: %f" % (
                    gesture.id, self.state_names[gesture.state],
                    swipe.position, swipe.direction, swipe.speed)

            if gesture.type == Leap.Gesture.TYPE_KEY_TAP:
                keytap = KeyTapGesture(gesture)
                print "  Key Tap id: %d, %s, position: %s, direction: %s" % (
                    gesture.id, self.state_names[gesture.state],
                    keytap.position, keytap.direction)

            if gesture.type == Leap.Gesture.TYPE_SCREEN_TAP:
                screentap = ScreenTapGesture(gesture)
                print "  Screen Tap id: %d, %s, position: %s, direction: %s" % (
                    gesture.id, self.state_names[gesture.state],
                    screentap.position, screentap.direction)

    def on_disconnect(self, controller):
        # Note: not dispatched when running in a debugger.
        print "Disconnected"

    def on_exit(self, controller):
        print "Exited"


def main():
    listener = DirectionListener()
    controller = Leap.Controller()

    controller.add_listener(listener)

    # Keep this process running until Enter is pressed
    print "Press Enter to quit..."
    try:
        sys.stdin.readline()
    except KeyboardInterrupt:
        pass
    finally:
        # Remove the sample listener when done
        controller.remove_listener(listener)


if __name__ == '__main__':
    main()
