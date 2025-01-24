#pragma once
#include "Memory/Memory.hpp"
#include "Texture.hpp"

namespace Cosmic
{

	enum class EAttachmentFlags : int16
	{
		None            = 0,
		SwapchainTarget = CS_BIT(0),
		Persistent      = CS_BIT(1),
	};

    enum class EAttachmentType
    {
        ColorAttachment,
        DepthAttachment,
        DepthStencilAttachment
    };

	struct AttachmentClearValue
	{
		AttachmentClearValue(float4 color)
			: ClearColor(color)
		{
		}

		AttachmentClearValue(float32 depth, uint32 stencil)
			: ClearDepth(depth), ClearStencil(stencil)
		{
		}

		AttachmentClearValue()
			: ClearColor({ 0.0f, 0.0f, 0.0f, 1.0f }), ClearDepth(1.0f), ClearStencil(0)
		{
		}

		float4  ClearColor;

		float32 ClearDepth;
		uint32  ClearStencil;
	};

	struct AttachmentInfo
	{
		uint2                      Size = { 0, 0 }; // 0, 0 is swapchain size
		uint32                     Samples;
		ETextureFormat             Format;
		BitFlags<EAttachmentFlags> Flags;
		EAttachmentType            Type;
		AttachmentClearValue       ClearValue = AttachmentClearValue();
		String                     Name;
	};

	using RenderPassRecordingCallback = std::function<void()>;

	class RenderPass : public IRefCounted
	{
	public:
		RenderPass(const String& name);
		virtual ~RenderPass() = default;

	public:
		void AddInputAttachment(const String& name);
		void SetDepthStencilInput(const String& name);

		void AddColorOutput(const AttachmentInfo& attachmentInfo);
		void SetDepthStencilOutput(const AttachmentInfo& attachmentInfo);

	public:
		const Vector<AttachmentInfo>& GetOutputColorAttachments() const { return mColorOutputs; }

		AttachmentInfo&       GetOutputColorAttachment(uint32 index)       { return mColorOutputs[index]; }
		const AttachmentInfo& GetOutputColorAttachment(uint32 index) const { return mColorOutputs[index]; }

		AttachmentInfo&       GetDepthStencilAttachment()       { return mDepthStencilOutput; }
		const AttachmentInfo& GetDepthStencilAttachment() const { return mDepthStencilOutput; }

	public:
		void SetRecordingCallback(RenderPassRecordingCallback callback) { mCallback = callback; }

	public:
		const String& GetName() const { return mName; }
		uint2 GetSize() const { return mColorOutputs[0].Size; };

	protected:
		Vector<AttachmentInfo> mColorOutputs;
		AttachmentInfo mDepthStencilOutput;

		RenderPassRecordingCallback mCallback;
		String mName;
	};

#if 0
	class RenderGraph : public IRefCounted
	{
	public:
		void AddPass(const RenderPassInfo& passInfo, std::function<void()> callback);
		void Execute();

	private:
		Vector<Ref<RenderPass>> mRenderPasses;
	};
#endif


}
