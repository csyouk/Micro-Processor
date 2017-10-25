


void Touch_Init(void){
	Touch_ISR_Enable(1);
}
void Camera_Init(void){
	CAM_Set_Capture_Status(0);
	CAM_Capture_Run(0,0,0);
	CAM_Capture_Run(1, Cam_Width, Cam_Height);
	CAM_XY_Flip(0,1);

	Key_ISR_Enable(1);
	CAM_Interrupt_Enable(1);
}
