<div align="center">

<img src="https://via.placeholder.com/120x120/7C3AED/FFFFFF?text=⚡" width="120" height="120" alt="PromptEra Logo"/>

# PromptEra

### The World's Biggest Open-Source AI Content Creation Hub

**Built for 500M+ creators across India & South Asia who deserve tools in their language.**

[![License: MIT](https://img.shields.io/badge/License-MIT-7C3AED.svg)](https://opensource.org/licenses/MIT)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](CONTRIBUTING.md)
[![Made in India](https://img.shields.io/badge/Made%20in-India%20🇮🇳-FF9933)](https://github.com/yourusername/promptera)
[![Stars](https://img.shields.io/github/stars/yourusername/promptera?color=F59E0B)](https://github.com/yourusername/promptera/stargazers)
[![Issues](https://img.shields.io/github/issues/yourusername/promptera)](https://github.com/yourusername/promptera/issues)

[🌐 Live Demo](https://promptera.com) · [📖 Docs](https://promptera.com/docs) · [🐛 Report Bug](https://github.com/yourusername/promptera/issues) · [✨ Request Feature](https://github.com/yourusername/promptera/issues)

---

> *"Content creation ki navi era. Generate unlimited AI prompts for every niche, every platform, every language."*

</div>

---

## 🌍 The Problem We're Solving

**80 million Indian content creators. Less than 5% have access to proper AI tooling in their language.**

Every existing tool — PromptBase, Jasper, Copy.ai, ContentStudio — was built for English-speaking Western creators. They don't understand:
- The Indian creator's niche (finance in Gujarati, fitness in Hindi, comedy in Hinglish)
- Regional platform behavior (Instagram Reels strategy differs massively in India vs USA)
- Cultural nuance (Diwali content, cricket hooks, desi humor formats)
- Language needs (Hindi, Gujarati, Marathi, Tamil, Bengali)

**PromptEra is the first AI content hub built specifically for South Asian creators — and open source so the community can grow it.**

---

## ✨ What is PromptEra?

PromptEra is an **open-source, AI-powered content creation platform** where any creator can:

- 🎯 **Generate 50-100 unique content prompts** in 10 seconds for their exact niche
- 📱 **Get platform-specific content** for Instagram Reels, YouTube Shorts, TikTok, LinkedIn, Blogs, Podcasts
- 🌐 **Work in their language** — English, Hindi, Gujarati, Marathi (more coming)
- 🤖 **Get AI-ready prompts** for ChatGPT, Midjourney, DALL-E, Sora, Stable Diffusion
- 📅 **Build a 30-day content calendar** in minutes
- 🔥 **Find viral angles** for any topic instantly
- 📝 **Generate full video scripts** for YouTube and Instagram Reels

---

## 🚀 Features

### Core Features
| Feature | Description |
|---------|-------------|
| **⚡ Auto-Prompt Engine** | Generate 50-100 niche-specific prompts in 10 seconds using Claude AI |
| **📚 Prompt Library** | 10,000+ curated prompts across 500+ niches |
| **🎯 Niche Explorer** | Visual galaxy of 500+ niches with competition analysis |
| **🌐 Multi-Language** | English, Hindi, Gujarati, Marathi — more via community PRs |
| **🔥 Hook Generator** | 10 proven viral hook formulas applied to any topic |
| **#️⃣ Hashtag Universe** | Niche + platform specific hashtag strategy |
| **🎬 Script Generator** | Full YouTube/Reels scripts with B-roll suggestions |
| **📅 Content Calendar** | 30-day content plan with daily topics and hooks |
| **🖼️ Thumbnail Prompts** | Midjourney/DALL-E ready thumbnail generation prompts |
| **📊 Viral Angle Finder** | 5-7 different viral angles for any content topic |
| **👥 Community Prompts** | User-submitted, voted, and curated prompts |

### Supported Platforms
- 📸 Instagram (Reels, Posts, Stories, Carousels)
- 📺 YouTube (Videos, Shorts, Scripts)
- 🎵 TikTok
- 👔 LinkedIn
- ✍️ Blog / Long-form
- 🎙️ Podcast
- 📘 Facebook

### AI Tools Supported
- ChatGPT / GPT-4
- Claude (Anthropic)
- Midjourney
- DALL-E
- Stable Diffusion
- Google Gemini
- Sora (Video)
- Adobe Firefly

---

## 🏗️ Tech Stack

```
Frontend  : React 18 + Vite + Tailwind CSS + Framer Motion
Backend   : Node.js + Express.js
Database  : MySQL + Prisma ORM
AI Engine : Claude API (Anthropic) — Primary
Payments  : Razorpay (India) + Stripe (International)
Storage   : Cloudinary
Email     : Brevo SMTP
Auth      : JWT + Google OAuth
Hosting   : VPS + cPanel (self-hostable)
```

---

## 🛠️ Self-Hosting (Get Started in 5 Minutes)

### Prerequisites
- Node.js 18+
- MySQL 8.0+
- Claude API key (Anthropic)

### Quick Start

```bash
# Clone the repository
git clone https://github.com/yourusername/promptera.git
cd promptera

# Install dependencies
cd backend && npm install
cd ../frontend && npm install

# Setup environment variables
cp backend/.env.example backend/.env
# Edit .env with your API keys

# Setup database
cd backend
npx prisma migrate dev

# Seed initial data (1000+ prompts)
npm run seed

# Start development servers
npm run dev          # Backend on :3001
cd ../frontend
npm run dev          # Frontend on :5173
```

### Environment Variables

```env
# Required
DATABASE_URL=mysql://user:password@localhost:3306/promptera_db
ANTHROPIC_API_KEY=sk-ant-...
JWT_SECRET=your-secret-key

# Optional (for full features)
RAZORPAY_KEY_ID=rzp_live_...
CLOUDINARY_CLOUD_NAME=...
GOOGLE_CLIENT_ID=...
```

---

## 📁 Project Structure

```
promptera/
├── frontend/                 # React + Vite application
│   ├── src/
│   │   ├── pages/           # Route pages
│   │   ├── components/      # Reusable components
│   │   ├── store/           # Zustand state management
│   │   ├── services/        # API service layer
│   │   └── utils/           # Helpers and constants
│   └── public/
│
├── backend/                  # Node.js + Express API
│   ├── src/
│   │   ├── routes/          # API routes
│   │   ├── controllers/     # Business logic
│   │   ├── middleware/      # Auth, rate limiting
│   │   ├── services/        # AI, email, payment services
│   │   ├── jobs/            # Cron jobs
│   │   └── prisma/          # Database schema
│   └── scripts/             # Seed scripts
│
└── docs/                     # Documentation
```

---

## 🌐 API Reference

PromptEra exposes a public API for developers:

```bash
# Generate prompts
POST /api/generate
{
  "niche": "fitness",
  "platform": "instagram",
  "contentType": "hook",
  "tone": "motivational",
  "language": "hindi",
  "count": 50
}

# Get prompt library
GET /api/prompts?niche=fitness&platform=instagram&page=1

# Get niches
GET /api/niches

# Get trending
GET /api/trending?period=daily
```

Full API documentation: [promptera.com/docs/api](https://promptera.com/docs/api)

---

## 🤝 Contributing

**We especially welcome contributions for:**

- 🗣️ **New language support** — Tamil, Bengali, Telugu, Kannada, Punjabi
- 🎯 **New niche prompt packs** — region-specific niches
- 🌍 **Platform-specific strategies** — what works in your country
- 🐛 **Bug fixes and improvements**
- 📖 **Documentation improvements**

See [CONTRIBUTING.md](CONTRIBUTING.md) for full guidelines.

### How to Contribute

```bash
# Fork the repo
# Create your feature branch
git checkout -b feature/tamil-language-support

# Make your changes
# Add your prompts to: backend/src/data/prompts/tamil/

# Commit
git commit -m "feat: add Tamil language prompt support"

# Push and open a Pull Request
git push origin feature/tamil-language-support
```

---

## 🗺️ Roadmap

- [x] Core prompt library (10,000+ prompts)
- [x] Auto-generate engine (Claude AI)
- [x] Multi-language: English, Hindi, Gujarati
- [ ] Script Generator (Full)
- [ ] Content Calendar Builder
- [ ] Mobile App (React Native)
- [ ] Tamil, Bengali, Marathi language packs
- [ ] Browser Extension (Chrome)
- [ ] Brand Deal Marketplace
- [ ] Public API v1
- [ ] 1 Million prompts milestone

---

## 🌟 Why Open Source?

The Indian creator ecosystem is massive but fragmented. No single company can build niche prompt packs for 22 official Indian languages, understand the nuances of Gujarati humor, or know what hooks work for Tamil YouTube creators.

**But the community can.**

By making PromptEra open source:
- Tamil developers can add Tamil prompt packs
- Bengali creators can contribute Bengali content strategies
- Regional marketers can add hyper-local niche content
- Developers across India can self-host for their communities

This is a project **by Indian creators, for Indian creators** — and everyone else.

---

## 📊 Impact

| Metric | Target (12 months) |
|--------|-------------------|
| Registered Creators | 1,000,000 |
| Prompts in Library | 1,000,000 |
| Languages Supported | 10 |
| Countries Active | 15 |
| Community Contributors | 500+ |
| GitHub Stars | 5,000+ |

---

## 📄 License

MIT License — free to use, modify, and distribute.
See [LICENSE](LICENSE) for details.

---

## 🙏 Acknowledgments

- Built with [Claude](https://anthropic.com) by Anthropic — the AI that makes regional language generation actually work
- Inspired by every Indian creator who deserved better tools
- Community contributors who add languages and niches

---

<div align="center">

**⭐ Star this repo if you believe Indian creators deserve world-class tools ⭐**

Made with ❤️ in India

[promptera.com](https://promptera.com) · [@promptera](https://instagram.com/promptera) · [hello@promptera.com](mailto:hello@promptera.com)

</div>
