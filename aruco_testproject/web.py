import asyncio
import webrtc


async def main():
    pc = webrtc.RTCPeerConnection()

    stream = webrtc.get_user_media()
    for track in stream.get_tracks():
        pc.add_track(track, stream)

    local_sdp = await pc.create_offer()
    print(local_sdp.sdp)


if __name__ == '__main__':
    asyncio.run(main())
