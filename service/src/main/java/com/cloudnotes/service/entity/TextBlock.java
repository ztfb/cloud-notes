package com.cloudnotes.service.entity;

import lombok.Data;

import javax.persistence.*;

@Entity
@Data
@Table(name = "text_block")
public class TextBlock {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private int id;
    @Column(name = "text")
    private String text;
    @Column(name = "user_id")
    private int userId;
    @Column(name = "note_id")
    private int noteId;
}
